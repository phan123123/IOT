/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package example;

/**
 *
 * @author Ngoc
 */
import java.io.File;
import org.apache.commons.io.FileUtils;
import java.io.IOException; 

import com.opensymphony.xwork2.ActionSupport;

public class UpLoadFile extends ActionSupport {
   private File myFile;
   private String myFileContentType;
   private String myFileFileName;
   private String destPath;

   public String execute() {
      /* Copy file to a safe location */
      destPath = "e:/";

      try {
         System.out.println("Src File name: " + myFile);
         System.out.println("Dst File name: " + myFileFileName);
     	    	 
         File destFile  = new File(destPath, myFileFileName);
         FileUtils.copyFile(myFile, destFile);
  
      } catch(IOException e) {
         e.printStackTrace();
         return "error";
      }

      return "success";
   }
   
   public File getMyFile() {
      return myFile;
   }
   
   public void setMyFile(File myFile) {
      this.myFile = myFile;
   }
   
   public String getMyFileContentType() {
      return myFileContentType;
   }
   
   public void setMyFileContentType(String myFileContentType) {
      this.myFileContentType = myFileContentType;
   }
   
   public String getMyFileFileName() {
      return myFileFileName;
   }
   
   public void setMyFileFileName(String myFileFileName) {
      this.myFileFileName = myFileFileName;
   }
}
