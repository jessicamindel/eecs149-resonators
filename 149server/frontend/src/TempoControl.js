import './App.css';
import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { PORT } from './App';

function TempoControl(debug = false) {
    const incrementTaps = () => {
        setTaps(taps + 1);
    }

    /* Returns the number of milliseconds elapsed since
     * the last tap, or a negative number for errors. */
    const timeElapsed = () => {
        if (lastTime === 0) {
            /* Return a negative number for timeouts. */
            //return -1;
        }
        const currentTime = Date.now();
        const timeSince = currentTime - lastTime;
        setDiff(timeSince);
        setLastTime(currentTime);
        if (timeSince < 100) {
            /* Debounce.
             * It's really hard to tap more than once
             * every couple hundred milliseconds.
             * It's highly unlikely that a user can 
             * keep up with such a high tempo.  */
            return -1;
        }
        return timeSince;
    }

    /* Converts a millisecond time interval to a BPM. */
    const millisecondsToBPM = (milliseconds) => {
        return 60 / (milliseconds / 1000);
    }

    /* Returns the BPM between the last tap and the current tap. */
    const twoPointBPM = () => {
        const elapsed = timeElapsed();
        if (elapsed < 0) {
            return -1;
        }
        return millisecondsToBPM(elapsed);
    }

    const updateBPM = () => {
        const TPM = twoPointBPM();
        const newBPM = ((bpm * taps) + TPM) / (taps + 1);
        incrementTaps();
        if (TPM === -1) {
            /* Error condition; restart. */
            setBPM(0);
            setTaps(0);
        } else if (taps === 1 || bpm < 30) {
            /* First tap. */
            setBPM(TPM);
        } else {
            /* A later tap. */
            setBPM(newBPM);
        }
    }

    const sendBPM = () => {
        axios.post(`http://localhost:${PORT}/tempo/` + parseInt(bpm))
        .then(response => {console.log(response)})
    }

    const [diff, setDiff] = useState(0);
    const [lastTime, setLastTime] = useState(0);
    const [taps, setTaps] = useState(0);
    const [bpm, setBPM] = useState(0);

    /* Runs on startup. */
    useEffect(() => {
        //setLastTime(Date.now());
    }, [])
    
    return (
        <div className="tempo">
            <p style={{lineHeight:0}}>
                {bpm > 30 ? bpm.toFixed(1) : "⏹"}
            </p>
            <p style={{lineHeight:0}}><i>BPM</i></p>
            {debug && 
                <div>
                    <hr />
                    <code> Debug information </code>
                    <p>Time since last tap: <code>{diff}</code>     </p>
                    <p>Time of last tap:    <code>{lastTime}</code> </p>
                    <p>Taps:                <code>{taps}</code>     </p>
                </div>
            }

            <hr style={{width: 150}} />
            <div className="stacked">
            <div className="side-by-side">
                <button onClick = {updateBPM} >
                    Tap
                </button>
                <hr style={{width: 2, margin: 7}} />
                <button onClick = {sendBPM}>
                    Set
                </button>
            </div>
            </div>
        </div>
    );
}

export default TempoControl;