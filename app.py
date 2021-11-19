from flask import Flask, render_template, session
import json
from scenariy.routes import menu_app
from scenar_auth.route import auth_app


app = Flask(__name__)
app.register_blueprint(auth_app, url_prefix='/auth')
app.register_blueprint(menu_app, url_prefix='/menu')

app.config['SECRET_KEY'] = 'mysecret'
app.config['ACCESS_CONFIG'] = json.load(open('configs/access.json'))

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/clear-session')
def clear_session():
    session.clear()
    return render_template('exit.html')

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5000)
