*****************************************************
*********************IOT-PROJECT*********************
-----------------------------------------------------
			Mục lục

Phần A: Tổng quan
1. websocket là gì?
2. hibernate framework
3. struts2 framework

Phần B: Server
 Mục I: Các file cấu hình
 Mục II: Các lớp Action
 Mục III: Lớp Util và Hibernate Mapping
 Mục IV: các lớp Pojo
 Mục V: Lớp xử lí yêu cầu websocket

Phần C: Quy ước
1. Dữ liệu: Json.

-----------------------------------------------------

Phần A: Tổng quan
	1. websocket là 1 giao thức có khả năng mở 1 socket giao tiếp
liên tục 2 chiều giữa server và client. Được sử dụng với mục đích
thời gian thực, khác với HTTP, websocket có Header nhẹ hơn nên tốc độ
truyền nhanh hơn.
	2. Hibernate là 1 Framework Java, là 1 giải pháp Object-
Relational Mapping (ORM), có nghĩa là ánh xạ các đối tượng thành các
hàng trong database. Hibernate hoạt động dựa trên file xml để ánh xạ 
các lớp trong java vào các bảng trong database. 
	3. Struts2 là framework phát triển trên Struts1, hoạt động trên 
mô hình MVC. một số công cụ của struts2: dispatcher web page, upload file,
DataBase access,...

-----------------------------------------------------

Phần B: Server
 Mục I: Ccác file cấu hình


 Mục II: các lớp Action
	1.Lớp Login: Lớp này xử lí yêu cầu đăng nhập của người dùng
	2.Lớp UploadFile: khi file đc upload, sẽ được lưu trũ trong 1 thư
mục tạm thời vì thế cần lưu sang 1 thư mục khác.


 Mục III: Lớp Util và Hibernate Mapping


 Mục IV: các lớp Pojo
	1. Lớp Device 
	2. Lớp User
 

 Mục V: Xử lí các yêu cầu websocket
	1. DeviceWebSocketServer
	 Đây là endpoint được định nghĩa bởi @ServerEndpoint annotation, phục vụ 
bắt các sự kiện trên server. sử dụng 4 annotation sau: @OnOpen, @OnClose,
@OnMessage, @OnError.

	2. MessageHandler
	 Xử lí các message từ phía client. Xác định yêu cầu theo trường "action"
từ message Json.

	3. SessionHandler
	Xử lí trực tiếp với các phiên làm việc của client.

-----------------------------------------------------

Phần C: Quy ước
	1. Dữ liệu message: Json