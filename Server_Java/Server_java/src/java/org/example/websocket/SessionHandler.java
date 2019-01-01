/*
*******************************************************************************
                           HEADER

                    (c)Copyright 2018 , NTP. Hanoi. VietNam
*Programer(s) : Ninh Thai Phan, Nguyen Tu Ngoc.
*Description  : Manage and handle the websocket sessions
*******************************************************************************
 */
package org.example.websocket;

/*
******************************************************************************
                            INCLUDE
 */
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;
import java.util.logging.*;
import javax.enterprise.context.ApplicationScoped;
import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonArrayBuilder;
import javax.json.JsonObject;
import javax.json.spi.JsonProvider;
import org.example.model.*;
import javax.websocket.Session;

/**
 * ***************************************************************************
 */
/*
*****************************************************************************
                            EXPORT

*removeSession                  :                                                       remove the session when disconnected
*getUserAdmin                    :                                                      return user admin
*getDeviceById(int id)          :                                                       return device by id
*addDevice(Device device)  :                                                        add device
*sendToSession(Session session, JsonObject message) :           send jsonMessage to session
*changeStatus(Session session) :                                                request device change status 
*sendFileToDevice(Session session, JsonObject jsonMessage) : send a path of file to device when device request
*sendFile(JsonObject jsonMessage) :                                          send file to device when user request
                                                                                                        $fileName  = the file which want to send 
                                                                                                                            in E:LISTFILES
*sendListFile(Session session, JsonObject jsonMessage) :          send list of files in "E:/LISTFILES"  
*sendListDevice(Session session, ArrayList<Device> listDevice) : send list of devices to user
*JsonObject creatJsonFromDevice(Device dv,int check) :             create jsonObject from device's information

******************************************************************************
 */
/**
 * ***************************************************************************
 * BODY
 * ***************************************************************************
 */
/**
 *
 * @author Phan
 */
@ApplicationScoped
public class SessionHandler {

    private ArrayList<User> users;
    /*list users are connecting*/
    private ArrayList<Device> devices;
    /*list devices are connecting*/
    private User admin;

    public SessionHandler() {
        users = new ArrayList<User>();
        devices = new ArrayList<Device>();
    }

    public void removeSession(Session session) {
        try {
            for (Device device : devices) {
                if (device.getSession().getId().equals(session.getId())) {
                    devices.remove(device);
                }
            }
        } catch (Exception ex) {
            Logger.getLogger(SessionHandler.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            for (User user : users) {
                if (user.getSession().getId().equals(session.getId())) {
                    users.remove(user);
                }
            }
        } catch (Exception ex) {
            Logger.getLogger(SessionHandler.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            session.close();
        } catch (Exception ex) {
            Logger.getLogger(SessionHandler.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public User getUserAdmin() {
        return this.admin;
    }

    public Device getDeviceById(int id) {
        for (Device dv : devices) {
            if (dv.getId() == id) {
                return dv;
            }
        }
        return null;
    }

    public void addDevice(Device device) {
        devices.add(device);
    }

    public void errorAdd(Session session) {
        JsonProvider provider = JsonProvider.provider();
        JsonObject erorrMessage = provider.createObjectBuilder()
                .add("action", "error")
                .add("data", "license")
                .build();
        sendToSession(session, erorrMessage);
    }

//  update key moi cho moi thiet bi khi ket noi
    public void updateConnectedDevice(Session session, String licenseCode) {
        JsonProvider provider = JsonProvider.provider();
        JsonObject setLicenseMessage = provider.createObjectBuilder()
                .add("action", "setLicense")
                .add("license", licenseCode)
                .build();
        sendToSession(session, setLicenseMessage);
    }

    public void sendToSession(Session session, JsonObject message) {
        try {
            session.getBasicRemote().sendText(message.toString());
        } catch (Exception ex) {
            Logger.getLogger(SessionHandler.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

//    yeu cau thiet bi thay doi trang thai
    public void changeStatus(Session session) {
        JsonProvider provider = JsonProvider.provider();
        JsonObject changeStatusMessage = provider.createObjectBuilder()
                .add("action", "CHANGESTATUS")
                .build();
        sendToSession(session, changeStatusMessage);
    }

//    send file to device when device request
    public void sendFileToDevice(Session session, JsonObject jsonMessage) {
        BufferedReader bufferReader;
        try {
            File file = new File("e:/LISTFILES/" + jsonMessage.getString("file"));
            RandomAccessFile raf = new RandomAccessFile(file, "rw");

            String str = "";
            int sizeOfText = jsonMessage.getInt("sizetext");
            int dem = 0;
            int end = 0;
            int temp = -1;
            int temp1 = -1;
            int indByte = jsonMessage.getInt("byte");
            raf.seek(indByte);
            while (dem < sizeOfText && (temp = raf.read()) != -1) {
                str = str + (char) temp;
                dem++;
//                System.out.println((temp & ((1<<2) -1 )) << 6);
//                System.out.println(temp1  & ((1<<6) - 1));                             
            }
            if (indByte + dem - 1 >= 100000) {
                end = 1;
            }
            if (temp == -1) {
                end = 1;
            }
            JsonProvider provider = JsonProvider.provider();
            JsonObject fileMessage = provider.createObjectBuilder()
                    .add("action", "TEXT")
                    .add("sizetext", dem)
                    .add("text", str)
                    .add("file", file.getName())
                    .add("byte", indByte + dem - 1)
                    .add("end", end)
                    .build();

            sendToSession(session, fileMessage);
            System.out.println(fileMessage.toString());
            raf.close();
        } catch (IOException e) {

        }
    }

//    send file to device when user request
    public void sendFile(JsonObject jsonMessage) {
        try {
            File file = new File("e:/LISTFILES/" + jsonMessage.getString("fileName"));
            System.out.println(file.getAbsolutePath());
            RandomAccessFile raf = new RandomAccessFile(file, "rw");
            String str = "";
            int sizeOfText = 300;
            int dem = 0;
            int end = 0;
            int temp = -1;
            int temp1 = -1;
            int indByte = 0;
            raf.seek(indByte);
            while (dem < sizeOfText && (temp = raf.read()) != -1) {
                str = str + (char) temp;
                dem++;
//                System.out.println((temp & ((1<<2) -1 )) << 6);
//                System.out.println(temp1  & ((1<<6) - 1));                           
            }
            System.out.println(str);
            if (temp == -1) {
                end = 1;
            }
            JsonProvider provider = JsonProvider.provider();
            JsonObject fileMessage = provider.createObjectBuilder()
                    .add("action", "TEXT")
                    .add("sizetext", dem)
                    .add("text", str)
                    .add("file", file.getName())
                    .add("byte", indByte + dem - 1)
                    .add("end", end)
                    .build();
            System.out.println(fileMessage.toString());
            sendToSession(getDeviceById(jsonMessage.getInt("id")).getSession(), fileMessage);

            raf.close();
        } catch (IOException e) {

        }
    }

//    send list files to admin
    public void sendListFile(Session session, JsonObject jsonMessage) {
        JsonObject addMessage = null;
        JsonArrayBuilder builder = Json.createArrayBuilder();;
        JsonProvider provider = null;

        File folder = new File("e:/LISTFILES");
        File[] listFiles = folder.listFiles();
        for (int i = 0; i < listFiles.length; i++) {
            builder.add(listFiles[i].getName());
            provider = JsonProvider.provider();
        }

        addMessage = provider.createObjectBuilder()
                .add("action", "SHOWLISTFILE")
                .add("listFile", builder.build())
                .add("id", jsonMessage.getInt("id"))
                .build();
        sendToSession(session, addMessage);
    }

//    send list devices to user
    public void sendListDevice(Session session, ArrayList<Device> listDevice) {
        JsonProvider provider = JsonProvider.provider();
        JsonArrayBuilder builder = Json.createArrayBuilder();
        System.out.println("connected: " + devices.size());
        for (Device device : listDevice) {
            int check = 0;
            for (Device dvs : devices) {
                if (dvs.getId() == device.getId()) {
                    check = 1;
                    break;
                }
            }
            builder.add(creatJsonFromDevice(device, check));
        }
        JsonArray jsonArray = jsonArray = builder.build();
        JsonObject listDeviceMessage = provider.createObjectBuilder()
                .add("action", "SHOWDEVICES")
                .add("listDevice", jsonArray)
                .build();
        sendToSession(session, listDeviceMessage);
    }
    //    create jsonObject from device

    private JsonObject creatJsonFromDevice(Device dv, int check) {
        System.out.println("check:" + check);
        JsonProvider provider = JsonProvider.provider();
        JsonObject jSon = provider.createObjectBuilder()
                .add("id", dv.getId())
                .add("name", dv.getName())
                .add("status", dv.getStatus())
                .add("type", dv.getType())
                .add("description", dv.getDescription())
                .add("license", dv.getLicense())
                .add("kind", dv.getKind())
                .add("connected", check)
                .build();
        return jSon;
    }
}
