

export default function Bluetooth() {

    let options = {
        filters: [
          {services: [0x0694]},
          {namePrefix: 'Spike'},
          {namePrefix: 'SPIKE'},
          {namePrefix: 'LEGO'}
        ],
        optionalServices: ['battery_service']
    }
    options = {acceptAllDevices: true}

    function callBluetooth() {
        navigator.bluetooth.requestDevice(options).then(function(device) {
            console.log('Name: ' + device.name);
            // Do something with the device.
            return device.gatt.connect();
        })
            .catch(function(error) {
            console.log("Something went wrong. " + error);
        });
    }

    return (
        <div className="flex justify-center my-12">
            <button onClick={() => callBluetooth()} className="border rounded p-2 bg-gray-300"> Connect to Bluetooth </button>
        </div>
    )
}