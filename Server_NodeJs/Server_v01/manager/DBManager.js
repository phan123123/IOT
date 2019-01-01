var mongoose = require('mongoose');
var url = '';
var MyDevice = require('../object/Device.js');


function init(y_url) {
    url=y_url;
    mongoose.connect(url,{useNewUrlParser: true});
}

var Device =mongoose.model('Device',{name: String, type : String, description : String,license : String});
var User = mongoose.model('User',{username: String, pass:String});

function addDevice(dv) {
    let device = new Device({name:dv.getName(),type:dv.getType(),description: dv.getDescription(),license: dv.getLicense()});
    return device.save();
}

function removeDevice(id) {
    return Device.findOneAndRemove({_id: id})
}

function getDeviceById(id) {
    return Device.findOne({_id:id}).then((data)=>{ return convertDataToDeviceObject(data)});
}

function getDeviceByLicense(license) {
    return Device.find({license: license}).then((value)=>{return value[value.length-1]}).then((data)=>{return convertDataToDeviceObject(data)});
}

function getAllDevicesJsonData() {
    return Device.find({});
}

function updateDevice(id,data) {
    return Device.findOneAndUpdate({_id:id},{$set:data},{new: true});
}

// function getUserByUsername(username){
//     return User.findOne({username:username});
// }

// init('mongodb://127.0.0.1:27017/test');

// getAllDevices()
//     .then((value)=> {
//     console.log(value)})
//     .then(()=>{removeDevice('5bf50bc01830bf1ea0f53945')
//         .then(()=>{console.log('tet--------------------------')})
//         .then(()=>{getAllDevices().then((value)=>{
//             for(i=0;i<value.length;i++){console.log(value[i])};
//         })});});

// console.log(getDeviceById('5bf50bc01830bf1ea0f53945').toString());

function convertDataToDeviceObject(data){
    var dv = new MyDevice();
    try {
        dv.setId(data._id.toString());
        dv.setName(data.name);
        dv.setType(data.type);
        dv.setDescription(data.description);
        dv.setLicense(data.license);
        return dv;
    }catch (e) {
        return null;
    }
}

// init('mongodb://127.0.0.1:27017/test');
// dv = new MyDevice();
// dv.setName("123");
// dv.setType("test");
// dv.setLicense("123456789");
// dv.setDescription("thi nghiem");
// addDevice(dv).then((value)=>{console.log(value.name)});

// getDeviceById("5bff9e242901181f30a0a583").then((data)=>{data.creatNewLicense();updateDevice(data.getId(),{license: data.getLicense()})
//     .then((value)=>{
//         console.log(value);
//     })
// });
// getDeviceById("5bff9e242901181f30a0a583").then((data)=>{data.creatNewLicense();updateDevice(data.getId(),{license: data.getLicense()})
//     .then((value)=>{
//         console.log(value);
//     })
// });

module.exports = {
    init : init,
    addDevice : addDevice,
    removeDevice : removeDevice,
    getDeviceById : getDeviceById,
    getAllDevicesJsonData : getAllDevicesJsonData,
    getDeviceByLicense: getDeviceByLicense,
    updateDevice : updateDevice,
    // getUserByUsername : getUserByUsername
};