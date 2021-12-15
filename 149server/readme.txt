
Execute the following commands in this directory:

TERMINAL 1
python3 -m venv venv
. venv/bin/activate

pip install Flask
pip install -U flask-cors

export FLASK_APP=server
flask run

TERMINAL 2
cd frontend
npm install
npm install --save form-data
npm start

Scroll down to the bar at the bottom to see if the frontend is able to send & receive from the root endpoint of the backend.