var devices = [];
var users = [];

function add(device){
    devices.push(device);
}

function remove(socket) {
    for(let i;i<devices.length;i++){
        if(devices[i].getSocket() === socket){
            devices.splice(i,1);
            return;
        }
    }
    for( i =0 ;i<users.length;i++){
        if(users[i].getSocket() === socket){
            users[i].splice(i,1);
            return;
        }
    }
}

function removeById(id){
    for(let i=0;i<devices.length;i++){
        if(devices[i].getId() === id){
            devices.splice(i,1);
            return;
        }
    }
    for( i =0 ;i<users.length;i++){
        if(users[i].getId() === id){
            users[i].splice(i,1);
            return;
        }
    }
}


function sendToSocket(socket,JSonMsg) {
    try{socket.send(JSonMsg.toString())}catch (e) {};
}


function getDeviceById(id) {
    for(let i=0;i<devices.length;i++){
        if(devices[i].getId() === id ) return devices[i];
    }
    return null;
}


module.exports.add = add;
module.exports.remove = remove;
module.exports.removeById = removeById;
module.exports.getDeviceById = getDeviceById;
module.exports.sendToSocket = sendToSocket;