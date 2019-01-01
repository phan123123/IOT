/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.example.websocket;

/**
 *
 * @author V khuat
 */
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.enterprise.context.ApplicationScoped;
import javax.inject.Inject;
import javax.json.Json;
import javax.json.JsonObject;
import javax.json.JsonReader;
import javax.json.spi.JsonProvider;
import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;
import org.example.hibernate.Util;
import org.example.model.Device;
import org.example.model.User;
import java.io.File;
import java.io.RandomAccessFile;

@ApplicationScoped
@SuppressWarnings("unused")
@ServerEndpoint("/actions")
public class DeviceWebSocketServer {

//   @Inject
//    private MessageHandler msgHandle;
    static private MessageHandler msgHandle = new MessageHandler();

    @OnOpen
    public void open(Session session) {
        //session.setMaxIdleTimeout(5000);
        System.out.println("\n\n\n\n\n\n\nConneted:"+session.getId()+"\n\n\n\n\n\n\n");
    }

    @OnClose
    public void close(Session session) {
        System.out.println("\n\n\n\n\n\n\nDisconneted:"+session.getId()+"\n\n\n\n\n\n\n");
        msgHandle.removeSession(session);
    }

    @OnError
    public void onError(Throwable error) {
        System.out.println(error.toString());
        Logger.getLogger(DeviceWebSocketServer.class.getName()).log(Level.SEVERE, null, error);
    }

    @OnMessage
    public void handleMessage(String message, Session session) {
        msgHandle.handle(message,session);
    }
}
