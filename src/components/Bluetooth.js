import React from "react";

export const LEGO_HUB_UUID = 0xfd02

export default function Bluetooth() {

    let options = {
        filters: [
            {"manufacturerData":[{"companyIdentifier":919}]}
        ],
        optionalServices: [LEGO_HUB_UUID]
    }

    async function callBluetooth() {
        
        return navigator.bluetooth.requestDevice(options).then(function(device) {
            console.log('Name: ' + device.name);
            return device.gatt.connect();
        })
            .catch(function(error) {
            console.log("Something went wrong: " + error);
        });
    }

    function handleCharacteristicValueChanged(event) {
        const value = event.target.value;
        console.log(value);
    }

    function ab2str(buf) {
        return String.fromCharCode.apply(null, new Uint16Array(buf));
    }

    async function runBluetooth() {
        let connection = await callBluetooth();
        console.log(connection)
        let primaryConnection = await connection.getPrimaryService(LEGO_HUB_UUID);
        console.log(primaryConnection)
        //console.log(await primaryConnection.getCharacteristics())

        let characteristic = await primaryConnection.getCharacteristics()
        let temp = await characteristic[1];

        //var enc = new TextEncoder(); // always utf-8

        console.log("READING CHARACTERISTIC:")
        console.log(await temp);

        //console.log("WRITING")

        let notify = temp.startNotifications()
        temp.addEventListener('characteristicvaluechanged',
                                  handleCharacteristicValueChanged);
        console.log('Notifications have been started.');

        setTimeout(() => {
            connection.disconnect();
        }, 20000);

    }

    return (
        <div className="flex justify-center my-12">
            <button onClick={() => runBluetooth()} className="border rounded p-2 bg-gray-300"> Connect to Bluetooth </button>
        </div>
    )
}
