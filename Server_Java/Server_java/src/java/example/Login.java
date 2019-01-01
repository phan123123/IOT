package example;

import com.opensymphony.xwork2.ActionSupport;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.struts2.ServletActionContext;

public class Login  extends ActionSupport  {
    private String url = "";
    
    public String getUrl(){
            return url;
    }
    
    public String execute() throws Exception {
        String str = "error";     
        HttpServletRequest request = ServletActionContext.getRequest();
        HttpServletResponse respon = ServletActionContext.getResponse();
        
//        Cookie cookie = new Cookie("passWord", "password");
//        respon.addCookie(cookie);
        String passWord = request.getParameter("password");
//        Cookie ck[]  = null;
//        if((ck = request.getCookies()) != null)
//        System.out.println(ck[0].getValue());
        if(passWord.equals("admin")){
            url = "public_html/ListDevices.html";
            str =  "success";
        }
        else url = "Login.html";
        
        return str;
    }
}