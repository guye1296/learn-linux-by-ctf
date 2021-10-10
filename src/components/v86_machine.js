import * as React from "react"
import { useStaticQuery, graphql, Link } from "gatsby"
import {V86Starter} from "../v86/libv86"


export default class V86Machine extends React.Component {
    constructor(props) {
        super(props);

        // Create a ref that `V86Starter` can use
        this.screenContainerRef = React.createRef();
        this.emulator = new V86Starter({
            wasm_path: "https://github.com/copy/v86/releases/download/latest/v86.wasm",
            screen_container: this.screenContainerRef,
            // Using public URLs as a placeholder
            bios: {
                url: "https://github.com/copy/v86/raw/master/bios/seabios.bin"
            },
            vga_bios: {
                url: "https://github.com/copy/v86/raw/master/bios/vgabios.bin"
            },
            cdrom: {
                url: "https://github.com/copy/images/raw/master/linux.iso"
            },
            autostart: false,
        })
    }

    componentDidMount() {
        this.emulator.run();
    }

    componentWillUnmount() {
        this.emulator.stop();
    }

    render() {
        return 
            <div ref={this.screenContainerRef}>
                <div style="white-space: pre; font: 14px monospace; line-height: 14px"></div>
                <canvas style="display: none"></canvas>
            </div>
        ;
    }
}


