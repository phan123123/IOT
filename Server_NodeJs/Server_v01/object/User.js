class User{
    constructor (){
        this._id = null;
        this.username = null;
        this.pass = null;
        this.socket=null;
    }
    constructor(id, username, passwd){
        this._id = id; this.username= username; this.pass =passwd; this.socket = null;
    }

    getId(){return this._id;}
    setId(id){this._id=id;}
    getUserName(){return this.usernameame;}
    setUserName(name){this.username=name;}
    setSocket(socket){this.socket=socket;}
    getSocket(){return this.socket;}
    getPass(){return this.pass;}
    setPass(pass){this.pass=pass;}
}

module.exports = User;