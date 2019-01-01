var webSocket = new WebSocket('ws://192.168.8.100:8080/Server_java/actions');
var sideNavStatus = 'closed';
function toggleButton(id) {
    var button = document.getElementById(id);
    var status = button.innerHTML;
    var str = JSON.stringify({action: 'CHANGESTATUS'});
    var jSon = JSON.parse(str);
    webSocket.send(jSon);
    webSocket.onmessage = function (event) {
        var receive = event.data;
        if (receive.action === 'TOGGLE') {
            if (status === "On<a href=\"#\" onclick=\"toggleButton(" + id + ")\"> (Turn off)</a>")
            {
                button.innerHTML = "Off<a href=\"#\" onclick=\"toggleButton(" + id + ")\"> (Turn on)</a>";
            } else if (status === "Off<a href=\"#\" onclick=\"toggleButton(" + id + ")\"> (Turn on)</a>") {
                button.innerHTML = "On<a href = \"#\" onclick = toggleButton(" + id + ") > (Turn off)</a>";
            }
        }
    };
}

function send(ID) {
    var str = JSON.stringify({action: 'SENDFILE', id: ID});
    webSocket.send(str);
}

function openNav(id) {
    if (sideNavStatus === 'closed') {
        if (screen.width <= 800) {
            document.getElementById("mySidenav").style.width = "30%";
        } else {
            document.getElementById("mySidenav").style.width = "15%";
        }
        sideNavStatus = 'opened';
        var str = JSON.stringify({action: 'GETLISTFILE', id: id});
        webSocket.send(str);
    } else if (sideNavStatus === 'opened') {
        document.getElementById("mySidenav").style.width = "0";
        sideNavStatus = 'closed';
    }
}
function display(device) {
    var table = document.getElementById("tableDevices");

    var tr = document.createElement("tr");
    table.appendChild(tr);

    var name = document.createElement("td");
    name.setAttribute("data-title", "Name");
    name.innerHTML = device.name;
    tr.appendChild(name);

    var status = document.createElement("td");
    status.setAttribute("data-title", "Status");
    status.setAttribute("id", device.id);
    if (device.connected === 1)
    {
        if (device.status === "On")
        {
            status.innerHTML = device.status + "<a href = \"#\" onclick = toggleButton(" + device.id + ") > (Turn off)</a>";
        } else if (device.status === "Off") {
            status.innerHTML = device.status + "<a href = \"#\" onclick = toggleButton(" + device.id + ") > (Turn on)</a>";
        }
    }
    tr.appendChild(status);

    var type = document.createElement("td");
    type.innerHTML = device.type;
    type.setAttribute("data-title", "Type");
    tr.appendChild(type);

    var description = document.createElement("td");
    description.innerHTML = device.description;
    description.setAttribute("data-title", "Description");
    tr.appendChild(description);

    var license = document.createElement("td");
    license.innerHTML = device.license;
    license.setAttribute("data-title", "Licensetion");
    tr.appendChild(license);

    var send = document.createElement("td");
    send.setAttribute("data-title", "Send");
    var sendButton = document.createElement("button");
    sendButton.innerHTML = "&#9776;";
    sendButton.setAttribute("class", "button");
    sendButton.setAttribute("onclick", "openNav(" + device.id + ")");
    if (device.connected === 1) {
        send.appendChild(sendButton);
    }
    tr.appendChild(send);
}

function sendFile(fileName, id) {
    var str = JSON.stringify({fileName: fileName, action: 'SENDFILE', id: id});
    webSocket.send(str);
    closeNav();
}

function displayFileList(listFile, id) {
    var menu = document.getElementById("mySidenav");
    var fc = menu.firstElementChild;
    while (fc !== null) {
        menu.removeChild(fc);
        fc = menu.firstElementChild;
    }
    var closeButton = document.createElement("a");
    closeButton.innerHTML = "&times;";
    closeButton.setAttribute("class", "closebtn");
    closeButton.setAttribute("href", "#");
    closeButton.setAttribute("onclick", "closeNav()");
    menu.appendChild(closeButton);

    var file = listFile;

    for (var i = 0; i < file.length; i++)
    {
        var a = document.createElement("a");
        a.setAttribute("href", "#");
        a.setAttribute("class", "file");
        a.setAttribute("onclick", "sendFile(\"" + file[i] + "\"," + id + ")");
        a.innerHTML = file[i];
        menu.appendChild(a);
    }
}

window.onload = function () {
//    var xhttp;
//    if (window.XMLHttpRequest) {
//        xhttp = new XMLHttpRequest();
//    } else {
//        //tao doi tuong ActiveXObject neu trinh duyet khong ho tro XMLHttpRequest
//        xhttp = new ActiveXObject("Microsoft.XMLHTTP");
//    }
//    xhttp.onreadystatechange = function () {
//        if (xhttp.readyState === 4 && xhttp.status === 200)
//        {
//            var device = JSON.stringify(xhttp.responseText);
//            display(device);
//        }
//    };
//    xhttp.open("POST", "URL", true);
//    xhttp.send();
//    var str = {"name": "Bong den", "status": "On", "type": "Rang dong", "description": "Hinh tron", "license": "Accepted", "kind": "Don't know", "id": "1"};
//    var str2 = {"name": "Laptop", "status": "Off", "type": "Dell", "description": "14inch", "license": "Accepted", "kind": "Huynh", "id": "4"};
//    display(str);
//    display(str2);
//    var file = {"file": ["keylogger", "trojan", "virus"]};
//
//    displayFileList(file);
//
// 
};

webSocket.onopen = function () {
    alert('Opened connection websocket');
    var str = JSON.stringify({action: 'GETLISTDEVICE'});
    webSocket.send(str);
};

webSocket.onmessage = function (event) {
    var data = JSON.parse(event.data);
    if (data.action === 'SHOWDEVICES') {
        var list = data.listDevice;
        for (var i = 0; i < list.length; i++) {
            display(list[i]);
        }
    } else if (data.action === 'SHOWLISTFILE') {
        displayFileList(data.listFile, data.id);
    }
};



function closeNav() {
    document.getElementById("mySidenav").style.width = "0";
    sideNavStatus = 'closed';
}
