/*
*******************************************************************************
                           HEADER

                    (c)Copyright 2018 , NTP. Hanoi. VietNam
*Programer(s) : Ninh Thai Phan, Nguyen Tu Ngoc.
*Description  : Contact to database
*******************************************************************************
 */
package org.example.hibernate;

/*
******************************************************************************
                            INCLUDE
 */
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Random;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.json.JsonObject;
import javax.json.spi.JsonProvider;
import org.hibernate.SessionFactory;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import org.example.model.Device;
import org.example.model.User;
import org.example.websocket.DeviceWebSocketServer;
import org.hibernate.Hibernate;
import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.hibernate.Transaction;

/*
***************************************************************************
 */

 /*
*****************************************************************************
                            EXPORT


*getSessionFactory()  :             return sessionFactory
*getListDevice          :               return the list of all devices in database
*getListUser            :                return the list of all users in database
*getDevice(id)          :               return the object device have the id
*addDevice(device)      :           push the device to database
*removeDevice(id)       :            delete the device have the id from database
*updateDevice(id,data)  :          set the information of device in database
                                                $data: key+" "+value
                                                =>set the key of device by the value
*getLastDeviceId        :             return int of id of last device in database
*getUser(id)            :                return the object user have the id
*removeUser(id)         :             delete the user have the id from database
*addUser(user)          :             push the user to database
*setDevicesForUser(id,list):      set the list of user have the id
*getDevicesForUser(id)  :         return the list devices,which the user manage
*getLastUserId          :              return int of id of last user in database
*isExist(String license) :            check whether license is exist
*toggle(id)                 :              change status of device in DataBase
******************************************************************************
 */
/**
 * ************************************************************************
 * BODY
 *
 * @author Phan
 * *************************************************************************
 */
public class Util {

    private static SessionFactory sessionFactory;
    private StringBuffer license = new StringBuffer("KZSEIZGVQQEBCEQN");

    public SessionFactory getSessionFactory() {
        return sessionFactory;
    }

//    get license to register
    public String getLicense() {
        return this.license.toString();
    }

    //return list device in data base
    public ArrayList<Device> getListDevice() {
        try {
            Configuration configuration = new Configuration().configure();
            StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                    applySettings(configuration.getProperties());
            sessionFactory = configuration.buildSessionFactory(builder.build());
            return (ArrayList<Device>) sessionFactory.openSession().createCriteria(Device.class).list();
        } catch (Exception e) {
            System.out.println(e.toString());
        }
        return new ArrayList<Device>();
    }

    public ArrayList<User> getListUser() {
        try {
            Configuration configuration = new Configuration().configure();
            StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                    applySettings(configuration.getProperties());
            sessionFactory = configuration.buildSessionFactory(builder.build());
            return (ArrayList<User>) sessionFactory.openSession().createCriteria(User.class).list();
        } catch (Exception e) {
            System.out.println(e.toString());
        }
        return new ArrayList<User>();
    }

    //get Device by id
    public Device getDevice(int id) {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        try {
            Device device = (Device) session.get(Device.class, id);

            return device;
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            session.close();
        }
        return null;
    }

    public int getLastDeviceId() {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        try {
            List list = session.createCriteria(Device.class).list();
            return ((Device) list.get(list.size() - 1)).getId();
        } catch (Exception e) {
            System.out.println(e.toString());
            return 0;
        } finally {
            session.close();
        }
    }

    public int getLastUserId() {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        try {
            List list = session.createCriteria(User.class).list();
            return ((User) list.get(list.size() - 1)).getId();
        } catch (Exception e) {
            System.out.println(e.toString());
            return 0;
        } finally {
            session.close();
        }
    }

        public Device getLastDevice() {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        try {
            List list = session.createCriteria(Device.class).list();
            return (Device) list.get(list.size() - 1);
        } catch (Exception e) {
            System.out.println(e.toString());
            return null;
        } finally {
            session.close();
        }
    }
    
//  add one device to DataBase
    public Device addDeviceToDataBase(Device device) {
        try {         
                device.setLicense(getNewLicense());
                device.setId(getLastDevice().getId()+1);
                device.setStatus("on");
                Configuration configuration = new Configuration().configure();
                StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                        applySettings(configuration.getProperties());
                sessionFactory = configuration.buildSessionFactory(builder.build());
                Session session = sessionFactory.openSession();
                Transaction tx = null;
                Integer ID = null;
                try {
                    tx = session.beginTransaction();
                    ID = (Integer) session.save(device);
                    tx.commit();
                } catch (Exception e) {
                    System.out.println(e);
                } finally {
                    session.close();
                }
        } catch (Exception e) {
            System.out.println(e);
        }
        return device;
    }

//    ckeck device whether exist
    public int getDeviceIdByLicense(String license) {
        for (Device temp : getListDevice()) {
            if (license.equals(temp.getLicense())) {
                return temp.getId();
            }
        }
        return 0;
    }

    //  update key moi cho moi thiet bi khi ket noi
    public String updateConnectedDevice(int id) {
        String licenseCode = getNewLicense();
        System.out.println("new license code: "+licenseCode);
        updateDevice(id, "license " + licenseCode);
        return licenseCode;
    }

    //  Xoa key(khong cho them moi thiet bi)
    public void resetLicense() {
        this.license = new StringBuffer();
    }

//  Sinh key moi cho moi thiet bi
    private String getNewLicense() {
        StringBuffer newLicense = null;
        do{
             newLicense = new StringBuffer();
            for (int i = 0; i < 16; i++) {
                newLicense.append((char) rand(65, 90));
            }
            
        }
        while (newLicense.toString().equals(this.license));
        return newLicense.toString();
    }

//    random one interger from min to max
    public static int rand(int min, int max) {
        try {
            Random rn = new Random();
            int range = max - min + 1;
            int randomNum = min + rn.nextInt(range);
            return randomNum;
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
    }

    //xao 1 thiet bi khoi db
    public void removeDevice(int id) {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            Device device = (Device) session.get(Device.class, id);
            session.delete(device);
            tx.commit();
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            session.close();
        }
    }

    //update lai du lieu cua thiet bi trong db
    public void updateDevice(int id, String data) {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            Device device = (Device) session.load(Device.class, id);

            String[] request = data.split(" ", 2);
            if (request[0].equals("toggle")) {
                device.setStatus(request[1]);
            }
            if (request[0].equals("description")) {
                device.setDescription(request[1]);
            }
            if (request[0].equals("license")) {
                device.setLicense(request[1]);
            }

            session.update(device);
            tx.commit();
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            session.close();
        }
    }

    //lay ra user theo id
    public User getUser(int id) {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        try {
            return (User) session.get(User.class, id);
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            session.close();
        }
        return null;
    }

    //them user va db
    public void addUser(User user) {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            session.save(user);
            tx.commit();
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            session.close();
        }
    }

    //xoa user khoi db
    public void removeUser(int id) {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        Transaction tx = null;
        try {
            tx = session.beginTransaction();
            User user = (User) session.get(User.class, id);
            session.delete(user);
            tx.commit();
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            session.close();
        }
    }

    public boolean isExist(String license) {
        for (Device temp : this.getListDevice()) {
            if (license.equals(temp.getLicense())) {
                return true;
            }
        }
        return false;
    }

    public void setDevicesForUser(int id, Set<Integer> idDevices) {
        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        Transaction tx = null;
        User user = null;
//        User  = session.get(User.class, id);

        try {
            tx = session.beginTransaction();
            user = (User) session.get(User.class, id);
            Hibernate.initialize(user.getDevices());
            Set tempSet = new HashSet<>();
            for (Integer i : idDevices) {
                tempSet.add((Device) getDevice(i));
            }

            user.setDevices(tempSet);

            session.save(user);
            tx.commit();

        } catch (Exception e) {
            System.out.println(e);
        } finally {
            session.close();
        }
    }

    public Set getDevicesForUser(int id) {

        Configuration configuration = new Configuration().configure();
        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
                applySettings(configuration.getProperties());
        sessionFactory = configuration.buildSessionFactory(builder.build());
        Session session = sessionFactory.openSession();
        Transaction tx = null;
        User user = null;

        try {
            tx = session.beginTransaction();
            user = (User) session.get(User.class, id);
            Hibernate.initialize(user.getDevices());
            Set devices = user.getDevices();
            tx.commit();

            return devices;
        } catch (Exception e) {
            System.out.println(e);
        } finally {
            session.close();
        }

        return null;
    }

    public void toggle(int id) {
        Device device = getDevice(id);
        if (device != null) {
            if ("On".equals(device.getStatus())) {
                device.setStatus("Off");
                updateDevice(id, "toggle Off");
            } else {
                device.setStatus("On");
                updateDevice(id, "toggle On");
            }
        }
    }

    //    public void test() {
//        Configuration configuration = new Configuration().configure();
//        StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().
//                applySettings(configuration.getProperties());
//        sessionFactory = configuration.buildSessionFactory(builder.build());
//
////        Device device1 = new Device(5, "device2", "off", "do dien", "ok roi 2", "adfsd 22", "sdfasfa ");
//    
////        User user1 = new User("thien", "thien");
//
//
//
////        User user1 = getUser(25);
////        Device device1 = getDevice(3);
////        Device device2 = getDevice(1);
////        HashSet set1 = new HashSet();
////        set1.add(device1);
////        set1.add(device2);
//
//
////        updateDevice(1, "toggle off");
//        
//        Session session = sessionFactory.openSession();
//
////        user1.setDevices(set1);
//        Transaction tx = null;
//        User user = null;
//
////        try {
////
////            tx = session.beginTransaction();
////            session.update(user1);
////
////            tx.commit();
////        } catch (HibernateException e) {
////            if (tx != null) {
////                tx.rollback();
////            }
////            e.printStackTrace();
////        } finally {
////            session.close();
////        }
//
////        user = (User) session.get(User.class, 25);       
////        Hibernate.initialize(user.getDevices());
//        
//
//        Set devices = user.getDevices();
//
//        for (Object device : devices) {
//            Device dv = (Device) device;
//            System.out.println(device.toString());
//        }      
//        
//    }
}

/*/PAGE*/
