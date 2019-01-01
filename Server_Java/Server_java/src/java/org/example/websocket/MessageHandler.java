/*
*******************************************************************************
                           HEADER

                    (c)Copyright 2018 , NTP. Hanoi. VietNam
*Programer(s) : Ninh Thai Phan, Nguyen Tu Ngoc.
*Description  : Handle the messages
*******************************************************************************
 */
package org.example.websocket;

/*
******************************************************************************
                            INCLUDE
 */
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonArrayBuilder;
import org.example.hibernate.Util;
import org.example.model.*;
import javax.websocket.Session;
import javax.json.JsonObject;
import javax.json.JsonReader;
import javax.json.spi.JsonProvider;

/**
 * **************************************************************************
 */
/*
******************************************************************************
                            BODY
*****************************************************************************/
/**
 *
 * @author Phan
 */
public class MessageHandler {

    private int deviceId;
    /* id of new device if have*/
    private int userId;
    /* id of new user if have*/
    private Util util;
    private SessionHandler sessionHandle;

    final private String actions[] = {
        "ADD",
        "REMOVE",
        "TOGGLE",
        "CHANGESTATUS",
        "GETTEXT",
        "SENDFILE",
        "GETLISTFILE",
        "GETLISTDEVICE"
    };

    public MessageHandler() {
        util = new Util();
        sessionHandle = new SessionHandler();
        deviceId = util.getLastDeviceId();
        userId = util.getLastUserId();
    }

    public void handle(String mess, Session session) {
        Device device = new Device();
        int index = -1;
        JsonObject jsonMessage = null;
        try {
            JsonReader reader = Json.createReader(new StringReader(mess));
            jsonMessage = reader.readObject();
            String str = jsonMessage.getString("action");
            device = convertMsgToDevice(jsonMessage);
            for (int i = 0; i < actions.length; i++) {
                if (str.equalsIgnoreCase(actions[i])) {
                    index = i;
                    break;
                }
            }
        } catch (Exception e) {
        }
        switch (index) {
//            add device 
            case 0:
                add(session, jsonMessage);
                break;
//            remove device by id
            case 1:
                removeDeviceByID(device.getId());
                break;
//            toggle device when device request
            case 2:
                toggle(device.getId());
                break;
//            toggle device when admin request
            case 3:
                changeStatus(jsonMessage.getInt("id"));
                break;
//            send file to device when device request
            case 4:
                sendFileToDevice(session, jsonMessage);
                break;
//            send file to device when admin request
            case 5:
                sendFile(jsonMessage);
                break;
//            send list files to admin
            case 6:
                sendListFile(session, jsonMessage);
                break;
//            send list devices to admin
            case 7:
                getListDevice(session);
                break;
        }
    }

//    add device to DataBase if not exist in DataBase or add device to devices if exist in DataBase
    private void add(Session session, JsonObject jsonMessage) {
        Device dv ;
        if (util.getLicense().equals(jsonMessage.getString("license"))) {
            dv = util.addDeviceToDataBase(convertMsgToDevice(jsonMessage));
            sessionHandle.updateConnectedDevice(session, dv.getLicense());
            dv.setSession(session);
            sessionHandle.addDevice(dv);
        } else if (util.getDeviceIdByLicense(jsonMessage.getString("license"))!=0) {
            dv = convertMsgToDevice(jsonMessage);
            dv.setId(util.getDeviceIdByLicense(jsonMessage.getString("license")));
            String license = util.updateConnectedDevice(dv.getId());
            sessionHandle.updateConnectedDevice(session, license);
            dv.setSession(session);
            sessionHandle.addDevice(dv);
        }
    }

    public void removeSession(Session session){
        sessionHandle.removeSession(session);
    }
    
    private void removeDeviceByID(int id) {
        util.removeDevice(id);
        if (sessionHandle.getDeviceById(id) != null) {
            sessionHandle.removeSession(sessionHandle.getDeviceById(id).getSession());
        }
    }

//    doi trang thai cua thiet bi do nguoi dung yeu cau den thiet bi do
    private void changeStatus(int id) {
        Session session = util.getDevice(id).getSession();
        sessionHandle.changeStatus(session);
    }

//    thay doi trang thai cua thiet bi trong DataBase do thiet bi yeu cau roi gui toi admin
    private void toggle(int id) {
        util.toggle(id);
        JsonProvider provider = JsonProvider.provider();
        JsonObject changeStatusMessage = provider.createObjectBuilder()
                .add("action", "TOGGLE")
                .build();
        sessionHandle.sendToSession(sessionHandle.getUserAdmin().getSession(), changeStatusMessage);
    }

    private void getListDevice(Session session) {
        ArrayList<Device> listDevice = util.getListDevice();
        sessionHandle.sendListDevice(session, listDevice);
    }

//    
    /*get information in msg to device*/
    private Device convertMsgToDevice(JsonObject jSon) {
        Device device = new Device();
        try {
            device.setId(jSon.getInt("id"));
        } catch (Exception e) {
        }
        try {
            device.setName(jSon.getString("name"));
        } catch (Exception e) {
        }
        try {
            device.setType(jSon.getString("type"));
        } catch (Exception e) {
        }
        try {
            device.setDescription(jSon.getString("description"));
        } catch (Exception e) {
        }
        try {
            device.setLicense(jSon.getString("license"));
        } catch (Exception e) {
        }
        try {
            device.setStatus(jSon.getString("status"));
        } catch (Exception e) {
        }
        try{
            device.setKind(jSon.getString("kind"));
        }catch(Exception e){}
        return device;
    }

//    send file to device when device request
    private void sendFileToDevice(Session session, JsonObject jsonMessage) {
        sessionHandle.sendFileToDevice(session, jsonMessage);
    }

//    send file to device when user request
    private void sendFile(JsonObject jsonMessage) {
        sessionHandle.sendFile(jsonMessage);
    }

    private void sendListFile(Session session, JsonObject jsonMessage) {
        sessionHandle.sendListFile(session, jsonMessage);
    }
}
