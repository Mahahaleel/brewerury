# from flask import Flask
from app import app
# import os

# project_root = os.path.dirname(__file__)
# template_path = os.path.join(project_root, 'app/templates')
# app = Flask(__name__, template_folder=template_path)

if __name__ == "__main__":
    app.run()
