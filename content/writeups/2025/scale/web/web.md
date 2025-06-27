+++
date = '2025-04-28T12:00:00+02:00'
draft = false 
title = 'Web challenges'
tags = ['web']
+++

Two challenges solved in web 

# The dev robots

This involved downloading an exposed .git and using the git history to recover an admin password.

# Artist

This was a webpage with a function to change the background to any image using curl on the server.
```python
@app.route('/set_background', methods=['POST'])
def set_background():
    try:
        background = request.json.get('background', '')
        
        if not (background.startswith('#') or background.startswith('rgb') or background.replace(' ', '').isalpha()):
            try:
                result = subprocess.run(
                    ['curl', '-s', '-L', background],
                    capture_output=True,
                    shell=False
                )
                
                if result.returncode == 0 and result.stdout:
                    encoded = base64.b64encode(result.stdout).decode('utf-8')
                    return jsonify({
                        'background': f'data:image/png;base64,{encoded}'
                    })
                return jsonify({'background': ''})
```

This way of calling curl using subprocess is (as far as I know) immune to command injections attacks however we still control the url field.  
curl supports a really useful protocol called `file` which allows you to read any file on the system (as long as the use running the process as read permissions).  

So we can for example send `file:///etc/passwd` for the url of the background and we will receive the base64 encoded content of the file.  
Ok but this doesn't seem that useful at first because there is no `flag.txt` the flag is an environment variable.  
Something nice about linux is that a lot of informations on the current state of the different processes is avaible in the `/proc` pseudo filesystem. In that filesystem we have the directory `/proc/self` containing informations about the process that reads it.  
And furthermore in that directory is the file `/proc/self/environ` which when read returns all the environment variables for the current process.

So we send `file:///proc/self/environ` as the url for the background image and receive all the environment variables base64 encoded. Including the flag.
