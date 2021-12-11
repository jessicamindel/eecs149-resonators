from flask import Flask
from flask import request

app = Flask(__name__)

@app.route("/", methods=['GET', 'POST'])
def hello_world():
    if request.method == 'POST':
        return "<img src='https://miro.medium.com/focal/87/87/50/50/1*XBktsimICRx2AbHA3vRNYQ.jpeg' />"
    return "<p>Hello.</p> \
            <img src='https://miro.medium.com/focal/87/87/50/50/1*XBktsimICRx2AbHA3vRNYQ.jpeg' />"