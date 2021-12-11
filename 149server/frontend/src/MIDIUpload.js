import './App.css';
import React, { useEffect, useState } from 'react';
import axios from 'axios';

function MIDIUpload(file, callback, debug = false) {
    return (
        <div className="tempo">
            {/*<input type="file" input={file} onInput={callback} onDragOver={callback}></input>*/}
            <input type="file" onChange={callback}></input>
        </div>
    );
}

export default MIDIUpload;