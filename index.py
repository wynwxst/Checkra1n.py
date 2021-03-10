import os
from flask import Flask, render_template
app = Flask(__name__)

@app.route('/')
def index():
  return render_template('index.html')

@app.route('/Jailbreak/')
def Jailbreak():
  return 'Jailbreaking... check terminal for information'
  os.system("python ipwndfu -p")

if __name__ == '__main__':
  app.run(port="8080")