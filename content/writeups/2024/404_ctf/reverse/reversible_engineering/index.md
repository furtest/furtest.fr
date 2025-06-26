+++
date = '2024-05-14T19:00:00+02:00'
draft = false
title = 'Reversible engineering'
tags = [ "rev" ]
+++

**Mon programme ne fonctionne pas à tout les coups il faut le lancer plusieurs fois pour obtenir le flag si quelqu'un arrive à comprendre pourquoi n'hésitez pas à me contacter sur discord (furtest).**

# Description 

Après une année éprouvante marquée par les compétitions, vous décidez de rentrer dans votre village natal. C'est avec beaucoup d'émotion que vous apercevez le dojo de votre enfance et décidez de vous y rendre. Votre ancienne sensei vous y attend, le sourire aux lèvres.

"La clairvoyance est l'arme la plus redoutable du combattant. Anticiper chaque mouvement avant qu'il ne soit lancé, voilà la véritable maîtrise du combat. Relève mon défi et prouve ta valeur."

Récupérer une archive zip avec netcat contenant un crackme et un token, renvoyer le token avec la solution du crackme à un deuxième serveur, recevoir un flag... Facile. Petit détail : vous avez vingt secondes pour faire tout ça, et le binaire change à chaque essai.

Connexion :
nc challenges.404ctf.fr 31998 > chall.zip
nc challenges.404ctf.fr 31999


# Début de la réflexion

Le but du challenge est de récupérer un binaire, trouver le mot de passe permettant de valider et de renvoyer ce mot de passe pour obtenir le flag.

On commence par télecharger et analyser quelques binaires à l'aide de ghidra.

```C
int main(int argc,char **argv)
{
  int is_different;
  int return_code;
  size_t password_size;
  void *encrypted;
  undefined8 key_part_1;
  undefined8 key_part_2;
  int size;
  
  if (argc < 2) {
    puts("J\'ai besoin d\'un argument!");
    is_different = 1;
  }
  else {
    password_size = strlen(argv[1]);
    size = (int)password_size;
    if (size == 16) {
      key_part_1 = 0x7b3f3a454d58604d;
      key_part_2 = 0x39485a4069796e5b;
      encrypted = (void *)encrypt(argv[1]);
      is_different = memcmp(encrypted,&key_part_1,16);
      if (is_different == 0) {
        puts("GG!");
        return_code = 0;
      }
      else {
        puts("Dommage... Essaie encore!");
        return_code = 1;
      }
    }
    else {
      puts("L'argument doit comporter 16 caractères.");
      return_code = 1;
    }
  }
  return return_code;
}
```

On remarque que le programme est toujours similaire :
- On doit entrer un mot de passe de 16 caractères en argument.
- Ce mot de passe est chiffré et ce chiffrement est comparé avec une clé.

Mais il y a de petits changements :
- La clé change.
- Les opérations exécutées sur le mot de passe changent.

Analysons également quelques fonctions `encrypt`.
```c
char * encrypt(char *password)
{
  byte bVar1;
  byte bVar2;
  char *encrypted_password;
  int i;
  
  encrypted_password = (char *)malloc(16);
  for (i = 0; i < 0x10; i = i + 1) {
    bVar1 = password[i];
    bVar2 = bVar1 ^ (byte)(((int)(uint)bVar1 >> 5 & (int)(uint)bVar1 >> 3 & 1U) << 2) ^
            (byte)((bVar1 >> 6 & 1) << 2) ^ 0x10;
    bVar2 = bVar2 ^ (byte)(((uint)(bVar1 >> 7) & (int)(uint)bVar2 >> 2 & 1U) << 5) ^ 0x24;
    bVar2 = bVar2 ^ (byte)((bVar2 >> 3 & 1) << 2);
    bVar2 = bVar2 ^ bVar2 * '\x02' & 2 ^ 0x3a;
    encrypted_password[i] = bVar2 ^ bVar2 >> 3 & 1;
  }
  return encrypted_password;
}
```

On remarque que le mot de passe est chiffré caractère par caractère, il est donc possible de le bruteforce.

# Idée 

Mon idée est de récupérer le code de la fonction encrypt grâce à l'API de scripting de ghidra et de m'en servir pour dans un autre programme pour bruteforce le mot de passe.   
Pour cela, plusieurs choses sont nécessaires.

## Automatiser le télechargement des binaires

On écrit un petit script bash qui nous permettra également de lier entre elle toutes les parties du processus.    
On le complétera plus tard.
```bash
#!/bin/bash

rm -rf workdir 
mkdir workdir
cd workdir
mkdir ghtest

timeout 3s nc challenges.404ctf.fr 31998 > chall.zip
unzip chall.zip
cd ..
```

## Programme principal de bruteforce {#bruteforce}

On écrit un programme en C qui fera des appels à la fonction `encrypt` pour permettre le bruteforce.

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char * encrypt(char*);

int main(void){
    int count = 0;
	char passwd[17] = {0};
	char *res = NULL;
	int test;
    long int part1 = {key part 1 here}; 
    long int part2 = {key part 2 here}; 
	for(int c = 0; c < 16; c++){
		for(int i = 0; i < 256; ++i){
			passwd[c] = (char) i;
			res = encrypt(passwd);
			if(c < 9){
				test = memcmp(res, &part1, c+1);
			}else{
				test = memcmp(res+8, &part2, c-7);
			}
			if(test == 0){
                count++;
				printf("%c", (char) i);
				break;
			}
		}
	}
    if(count == 16){
        return 0;
    }
	return 1;
}
```

## Récupération de la fonction encrypt

On utilise python pour décompiler le crackme grâce à l'api de ghidra (headless analyzer) et on stocke le résultat dans deux fichiers :
- `decompiled_main.txt` pour pouvoir récupérer la clé 
- `decompiled_encode.c` qui contient le code pour la fonction `encrypt`    
(Voir [ici](https://github.com/HackOvert/GhidraSnippets) pour plus d'informations)

```python
from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

program = getCurrentProgram()
ifc = DecompInterface()
ifc.openProgram(program)

f = open("workdir/decompiled_main.txt", "at")
g = open("workdir/decompiled_encode.c", "at")
g.write("#include <stdlib.h>\n#include <stdint.h>") # On prépare le terrain pour la suite
fm = currentProgram.getFunctionManager()
funcs = fm.getFunctions(True)
for func in funcs: 
    if func.getName() == "FUN_00101169":
        f.write(ifc.decompileFunction(func, 0, ConsoleTaskMonitor()).getDecompiledFunction().getC())
    elif func.getName() == "FUN_0010123f":
        g.write(ifc.decompileFunction(func, 0, ConsoleTaskMonitor()).getDecompiledFunction().getC())
f.close()
g.close()
```

## Envoie du mot de passe 

On écrit un script python qui permettra d'envoyer le mot de passe une fois qu'il aura été récupéré.   
Au passage, on vérifie que le mot de passe est bien valide, car parfois, ce n'est pas le cas.   

```python
import socket

passwd_f = open("workdir/passwd.txt")
passwd = passwd_f.read()
passwd_f.close()
token_f = open("workdir/token.txt")
token = token_f.read()
token_f.close()

if len(passwd) != 16:
    print("Fail")
    quit()

print(f"Passwd : {passwd}")
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("challenges.404ctf.fr", 31999))
print(s.recv(4096).decode())
token += "\n"
s.send(token.encode())
print(s.recv(4096).decode())
passwd += "\n"
s.send(passwd.encode())
print(s.recv(4096).decode())
```

## Problème de la clé

On tombe rapidement sur un problème, il faut insérer la clé dans la fonction main du programme, on écrit donc un programme python permettant de générer le programme en C qui sera ensuite compilé avec `decompiled_encode.c` pour la fonction encrypt. (Il y a sûrement une autre façon de le faire.)    
Je ne remets pas tout le programme en C juste les lignes modifiées, vous pouvez le revoir [ici](#bruteforce).

```python
encoded_out = open("workdir/decompiled_main.txt")
content = encoded_out.readlines()


main = """
#{ coupé pour la visibilité }
    long int part1 = """ + content[20][17:-1] + """
    long int part2 = """ + content[21][17:-1] + """ 
#{ coupé pour la visibilité }
"""
encoded_out.close()

main_file = open("workdir/main.c", "wt")
main_file.write(main)
main_file.close()
```


# Lien entre les parties 

Il faut maintenant lier toutes les parties entre elles pour cela, on va modifier le script bash.
- On exécute le headless analyze de ghidra avec le script de décompilation.
- On utilise sed pour modifier un peu la fonction `encrypt` et pour que le compilateur soit content.
- On génére le programme principal de bruteforce.
- On compile tout puis on exécute en récupérant le mot de passe dans `passwd.txt`
- On envoie le mot de passe et on affiche le flag avec `send.py`

```bash
#!/bin/bash

rm -rf workdir 
mkdir workdir
cd workdir
mkdir ghtest

timeout 3s nc challenges.404ctf.fr 31998 > chall.zip
unzip chall.zip
cd ..
{ghidra_path}/support/analyzeHeadless workdir/ghtest testghidra -import workdir/crackme.bin -postScript decompile.py
sed -i 's/byte/char/g' workdir/decompiled_encode.c
sed -i 's/void/char/g' workdir/decompiled_encode.c 
sed -i 's/FUN_0010123f/encode/g' workdir/decompiled_encode.c
python3 bruteforce_gen.py
gcc workdir/main.c workdir/decompiled_encode.c -o workdir/bruteforce
./workdir/bruteforce > workdir/passwd.txt
python3 send.py
```

Plus qu'à exécuter le programme et hop là, c'est réussi.
