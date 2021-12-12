
import './App.css';
import MIDIUpload from './MIDIUpload';
import TempoControl from './TempoControl';
import Padding from './Padding';

import React, { useEffect, useState } from 'react';
import axios from 'axios';
import MediaControl from './MediaControl';

function App() {
  const [getMessage, setGetMessage] = useState({});

  useEffect(() => {
    axios.get('http://localhost:5000/').then(response => {
      console.log("Connecting to backend: ", response)
      setGetMessage(response)
    }).catch(error => {
      console.log("Connecting to backend: ", error)
    })
  }, [])

  const [MIDI, setMIDI] = useState(null);
  const uploadMIDI = (e) => {
    const midi = Array.from(e.target.files)[0];
    if (!midi) /* Don't upload a null MIDI */ return;
    setMIDI(midi);
  }
  useEffect(() => {
    /* Send MIDI to some endpoint. */
    if (!MIDI) /* Don't upload a null MIDI */ return;
    console.log("Uploading file from frontend:", MIDI);

    const reader = new FileReader();
    reader.onload = function () {
      axios({
        method: "post",
        url: "http://localhost:5000/midi",
        data: reader.result,
        headers: { 'Content-Type': 'application/json' },
      })
        .then((response) => {
          //handle success
          setGetMessage(response);
        })
        .catch((response) => {
          console.log(response);
        });
    }
    reader.readAsArrayBuffer(MIDI);
  }, [MIDI])

  const basicPost = (url) => {
    return () => {
      axios({
        method: "post",
        url: "http://localhost:5000/" + url
      })
        .then((response) => {
          setGetMessage(response);
        })
        .catch((response) => {
          setGetMessage(response);
          console.log(response);
        });
    }
  }

  return (
    <div className="App" style={{backgroundColor: 'beige'}}>
      <header className="App-header">
        {/*<img src={logo} className="App-logo" alt="logo" />*/}
        <div className="stacked">
          <div className="side-by-side">

            {MIDIUpload(uploadMIDI)}
            
          </div>

          {Padding(80)}

          <div className="side-by-side">

            {TempoControl(false)}

            {Padding(200)}

            {MediaControl(basicPost("play"), basicPost("stop"))}

          </div>
        </div>
        
        
      </header>
      {
            /* Tests backend integration. */
            <div>
              <p style={{textDecoration: "underline dotted"}}>
                {getMessage.status === 200 ? getMessage.data : "Unable to connect to backend."}
              </p>
            </div>
        }
    </div>
  );
}

export default App;