var manager = require('./SocketManager.js');
var Device = require('../object/Device.js');
var db = require('./DBManager.js');
const actions = ['ADD','REMOVE','TOGGLE','GET_LIST_DEVICES'];

const license='QAZXSWEDCVFRTGBN';
db.init('mongodb://127.0.0.1:27017/test');

function handleIO(socket){
    console.log('connected socket ID: '+socket.id);
    //handle the disconnect
    socket.on('disconnect',()=>{
        console.log('disconnected socket ID: '+socket.id);
        manager.remove(socket);
    });
    //handle the message
    socket.on('message', (data)=>{
        let jmsg;
        try{jmsg = JSON.parse(data);} catch(e){jmsg= data;}
        console.log(socket.id+" send: "+JSON.stringify(jmsg));
        let device = new Device();
        let index = -1;
        for(i=0;i<actions.length;i++){if(jmsg.action === actions[i]){index=i;break;}}
        try {
            device=convertJsonToDeviceObject(jmsg);
        }catch(e){ console.log(e.toString());}
        device.setSocket(socket);
        switch (index) {
            case 0: add(device); break;
            case 1: removeDevice(device.getId());break;
            case 2: toggle(device.getId()); break;
            case 3: getListDevices(socket);break;
            // case -1: socket.disconnect();break;
        }
    });
}


function add(device){
    if(device.getLicense()===license&&license.length>0){
        db.addDevice(device).then((value)=>{
            let dv = convertJsonToDeviceObject(value);
            dv.creatNewLicense();
            db.updateDevice(dv.getId(),{license:dv.getLicense()});
            let jsonMsg = new JSON;
            jsonMsg[setLicense]=dv.getLicense();
            manager.sendToSocket(device.getSocket(),jsonMsg);
            dv.setSocket(device.getSocket());
            manager.add(dv);
            console.log("Added :"+device.getName()+' id:'+dv.getId());
            // device.getSocket().clearTimeout();
        });
    }
    else if(license.length>0){
        db.getDeviceByLicense(device.getLicense()).then((value)=>{
            if(value){
                value.creatNewLicense();
                db.updateDevice(dv.getId(),{license:dv.getLicense()});
                manager.add(dv);
            }
            else{
                device.getSocket().disconnect();
            }
        })
    }
    else {device.getSocket().disconnect();}
    // db.addDevice(device).then(()=>{
    //     db.getDeviceByLicense(device.getLicense());
    //     manager.add(device);
    // });
}

function removeDevice(id) {
    manager.remove(id);
    db.removeDevice(id);
}

function toggle(id) {

}

function getListDevices(socket) {
    db.getAllDevices().then((value)=>{
        for(i=0;i<value.length;i++){
            if(manager.getDeviceById(value[i].getId())!=null){
                value[i][connected]=true;
            } else value[i][connected]=false;
        }
        manager.sendToSocket(socket, {'action': 'SHOW_DEVICES', 'listDevice': value});
    });
}

function creatLicense() {

}

function covertDeviceOjectToJson(device) {
    let json = new JSON();
    try{json[_id]=device.getId();}catch (e) {}
    try{json[name]=device.getName();}catch (e) {}
    try{json[description]=device.getDescription();}catch (e) {}
    try{json[type]=device.getType();}catch (e) {}
    try{json[status]=device.getStatus();}catch (e) {}
    try{json[license]=device.getLicense();}catch (e) {}
    return json;
}

function convertJsonToDeviceObject(JSonMsg){
    let device = new Device();
    try{device.setId(JSonMsg._id.toString());}catch(e){}
    try{device.setName(JSonMsg.name);}catch (e) {}
    try{device.setSocket(JSonMsg.socket);}catch (e) {}
    try{device.setDescription(JSonMsg.description);}catch (e) {}
    try{device.setType(JSonMsg.type);}catch (e) {}
    try{device.setLicense(JSonMsg.license);}catch (e) {}
    try{device.setStatus(JSonMsg.status);}catch (e) {}
    return device;
}

module.exports.handleIO = handleIO;