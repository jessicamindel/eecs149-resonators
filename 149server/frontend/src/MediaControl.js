
function MediaControl(play, pause) {
    return (
        <div className="stacked">
            <div className="side-by-side" style={{cursor: "default", minWidth: 80, minHeight: 40}}>
                <div style={{width: 40, height: 40}}>
                    <p onClick={play}>     ▶ </p>
                </div>

                <div style={{width: 40, height: 40}}>
                </div>

                <div style={{width: 40, height: 40}}>
                    <p onClick={pause}>    ⏹ </p>
                </div>

            </div>
        </div>
    );
}

export default MediaControl;