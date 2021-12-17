import * as React from "react"
import localforage from "localforage"


export default class V86Machine extends React.Component {
    constructor(props) {
        super(props);
        // Create a ref that `V86Starter` can use
        this.screenContainerRef = React.createRef();
        this.validCharacterCount = 0;
        this.initializationCompleteString = "ready4ctf";

        let emulatorSettings = {
            wasm_path: "/v86/v86.wasm",
            memory_size: 32 * 1024 * 1024,
            vga_memory_size: 2 * 1024 * 1024,
            autostart: false,
        };

        emulatorSettings.vga_bios = {
            url: "/bios/vgabios.bin"
        };

        emulatorSettings.bios = {
            url: "/bios/seabios.bin"
        };

        emulatorSettings.cdrom = {
            url: "/linux/v86-linux.iso"
        };

        this.state = {emulatorSettings: emulatorSettings, snapshot: null};

        localforage.getItem("linux4CTF_snapshot", function(err, value) {
            if(err) {
                throw(err);
            }

            if (value === null) {
                console.log("No snapshot found");
            } else {
                console.log("Found valid snapshot");
                this.emulator.valid = false;
                let settings = this.state.emulatorSettings;
                delete settings.bios;
                delete settings.cdrom;
                this.setState(
                    {emulatorSettings: settings, snapshot: value}, 
                    this.createEmulator.bind(this)
                );
            }
            this.isProfileInitialized = (this.snapshot != null);

        }.bind(this));
    }

    serial0Handler(char) {
        if (char === this.initializationCompleteString[this.validCharacterCount]) {
            this.validCharacterCount++;
        } else {
            this.validCharacterCount = 0;
        }
        // Reached the required length
        if (this.validCharacterCount === this.initializationCompleteString.length) {
            console.log("Usermode booted");

            // Remove this callback
            this.emulator.remove_listener(
                "serial0-output-char", 
                this.boundCallback
            );

            this.createLocalSnapshot();

            this.isProfileInitialized = true;
        }
    }


    createLocalSnapshot() {
        this.emulator.save_state(function(error, new_state) {
            if(error) {
                throw error;
            }

            console.log("saving snapshot (" + new_state.byteLength + " bytes)");
            localforage.setItem("linux4CTF_snapshot", new_state).then(function(value){
                console.log('Saved snapshot successfully');
            }).catch(function(error) {
                throw(error);
            });
        })
    }

    createEmulator() {
        // Stop running emulator if exists
        if (this.emulator) {
            this.emulator.stop();
            this.emulator = null;
        }

        this.emulator = new window.V86Starter(this.state.emulatorSettings);

        // Register a serial data callback function. As of now, 
        // the callback will only log data received from the serial port
        this.boundCallback = this.serial0Handler.bind(this);

        if (this.state.snapshot === null) {
            this.emulator.add_listener(
                "serial0-output-char",
                this.boundCallback,
            );
        }

        this.emulator.add_listener(
            "emulator-loaded",
            this.onEmulatorLoaded.bind(this),
        );
    }


    componentDidMount() {
        // The ref is only valid once the component had mounted
        let emulatorSettings = this.state.emulatorSettings;
        emulatorSettings.screen_container = this.screenContainerRef.current;
        this.setState({emulatorSettings: emulatorSettings, snapshot: this.state.snapshot});
        this.createEmulator();
    }

    onEmulatorLoaded() {
        // This condition is still vulnerable to some race condition -
        // the emulator could be valid from the first instanciation
        // but snapshot could be valid as well. Since the first instanciation
        // defined a CDROM I believe that the `restore_state` functionality
        // is not provided.
        // As a temporary workaround I've wrapped the `restore_state` and `run` in a 
        // `try` block.`
        // TODO: debug and fix
        try {
            if (this.emulator && this.state.snapshot) {
                console.log("Restoring state from local storage");
                    this.emulator.restore_state(this.state.snapshot);
            }
            console.log("Running emulator");
            this.emulator.run();
        } catch (e) {
            console.error("TODO: Fix");
        }
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


