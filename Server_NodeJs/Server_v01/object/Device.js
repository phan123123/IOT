class Device{
    constructor (){
        this._id = null;
        this.name = null;
        this.status = "Off";
        this.type=null;
        this.description=null;
        this.socket=null;
        this.license=null;
    }

    creatNewLicense(){
        this.license = (new Date().getTime()%100000).toString()+this._id.toString();
    }
    getId(){return this._id;}
    setId(id){this._id=id;}
    getName(){return this.name;}
    setName(name){this.name=name;}
    setSocket(socket){this.socket=socket;}
    getSocket(){return this.socket;}
    getType(){return this.type;}
    setType(type){this.type=type;}
    setDescription(description){this.description=description;}
    getDescription(){return this.description;}
    setStatus(status){this.status=status;}
    getStatus(){return this.status;}
    setLicense(license){this.license=license;}
    getLicense(){return this.license;}
}

module.exports = Device;