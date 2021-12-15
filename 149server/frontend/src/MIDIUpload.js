function MIDIUpload(callback, debug = false) {
    return (
        <div className="tempo">
            <input type="file" onChange={callback}></input>
        </div>
    );
}

export default MIDIUpload;