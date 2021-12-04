import logo from './logo.svg';
import './App.css';
import MIDIUpload from './MIDIUpload';
import TempoControl from './TempoControl';
import Padding from './Padding';

import React, { useEffect, useState } from 'react';
import axios from 'axios';

function App() {
  const [getMessage, setGetMessage] = useState({})

  useEffect(() => {
    axios.get('http://localhost:5000/flask/hello').then(response => {
      console.log("SUCCESS", response)
      setGetMessage(response)
    }).catch(error => {
      console.log(error)
    })
  }, [])

  const [MIDI, setMIDI] = useState(null);
  const uploadMIDI = (e) => {
    const midi = Array.from(e.target.files)[0];
    setMIDI(midi);
  }
  useEffect(() => {
    /* Send MIDI to some endpoint. */
    console.log(MIDI)
  }, [MIDI])

  return (
    <div className="App">
      <header className="App-header">
        {/*<img src={logo} className="App-logo" alt="logo" />*/}
        <div className="stacked">
          <div className="side-by-side">

            {MIDIUpload(MIDI, uploadMIDI)}
            

            {
            /* Tests backend integration.
            <div>{getMessage.status === 200 ?
              <h3>{getMessage.data.message}</h3>
              :
            <h3>LOADING</h3>}
            </div>*/
            }
          </div>

          {Padding(80)}

          <div className="side-by-side">

            {TempoControl(false)}

          </div>
        </div>
        
      </header>
    </div>
  );
}

export default App;