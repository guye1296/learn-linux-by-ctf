import * as React from "react"
import { useStaticQuery, graphql, Link } from "gatsby"


export default class V86Machine extends React.Component {
    constructor(props) {
        super(props);
        // Create a ref that `V86Starter` can use
        this.screenContainerRef = React.createRef();
    }

    componentDidMount() {
        this.emulator = new window.V86Starter({
            wasm_path: "/v86/v86.wasm",
            screen_container: this.screenContainerRef.current,
            memory_size: 32 * 1024 * 1024,
            vga_memory_size: 2 * 1024 * 1024,

            bios: {
                url: "/bios/seabios.bin"
            },
            vga_bios: {
                url: "/bios/vgabios.bin"
            },
            cdrom: {
                url: "/linux/v86-linux.iso"
            },
            autostart: true,
        });
        // TODO: figure out why `this.emulator.run()` does not work
    }

    componentWillUnmount() {
        this.emulator.stop();
    }

    render() {
        return (
            <div ref={this.screenContainerRef}>
                <canvas style={{display: 'none'}}></canvas>
                <div style={{whiteSpace: 'pre', font: '14px monospace', lineHeight: '14px'}}></div>
            </div>
        );
    }
}


