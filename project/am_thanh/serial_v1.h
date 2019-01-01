/********************************************************/
/*               asdcxsd v1.0 13/11/2018                 */
/********************************************************/

class Serial_make
{
  private:
    int pin_input; // cong
    int pin_frequency;
    int pin_full_sram;
    int frequency;
    String S, save_data;
    int sl_bit;
    int Bit[13];
    int size_max = 2; // size 
  public:
    void open_send(int pout, int pfre, int psram, int fre);
    void open_receive(int pin, int pfre , int psram);
    void receive_byte();
    void  write(String s);
    void send_byte(int x);
    int available();
    char read();
    int get_fre();
    void start_key();
    void change();
    void reset_stack();
    void pause_send(int x);
    
};
void Serial_make::send_byte(int x)
{
  int a[12];
  for(int i= 1; i<= 10; i++) a[i] = 0;
  a[0] = a[9] = 1;
  for(int i =1; x > 0; i++){
    a[i] = x%2;
    x /=2;
  }
  for(int i= 1; i < 9; i++)
  {
    int status_pin_input = LOW;
    if (a[i]) status_pin_input = HIGH;

    if (i != 1) digitalWrite(pin_input, LOW); // key start
    else digitalWrite(pin_input, HIGH); // key start

    digitalWrite(pin_frequency, HIGH);  // tang dien ap


    delayMicroseconds(frequency);

    digitalWrite(pin_input, status_pin_input);
    digitalWrite(pin_frequency, LOW);  // giam dien ap
    delayMicroseconds(frequency);

  }
}
void Serial_make::open_send(int pout, int pfre, int psram, int fre)
{
  this->pin_input = pout;
  this->pin_frequency = pfre;
  this->pin_full_sram = psram;
  this->frequency = fre;
  save_data = "";
  pinMode(pin_input, OUTPUT);
  pinMode(pin_frequency, OUTPUT);
  pinMode(pin_full_sram, INPUT);
  
  digitalWrite(pin_frequency, LOW);
}

void  Serial_make::write(String s)
{
  if (digitalRead(pin_full_sram) == HIGH) return;
  save_data += s;
  int len_data = 0;
  for(int i= 0; i<save_data.length(); i++)
    {
      if (digitalRead(pin_full_sram) == HIGH) 
      {
        Serial.println("pause");
        break;
      }
      send_byte((int)save_data[i]);
      len_data++;
    }
  save_data.remove(0, len_data);
  if (save_data != "") Serial.println(save_data);
}

void Serial_make::pause_send(int x)
{
  int k;
  if (x) 
  {
    k = HIGH;
    Serial.println("tang dien");
  }
  else  {
    k = LOW;
    Serial.println("giam dien");
  }
  
  digitalWrite(pin_full_sram, k);
}
void Serial_make::open_receive(int pin, int pfre, int fsram)
{
  this->pin_input = pin;
  this->pin_frequency = pfre;
  this->pin_full_sram = fsram;
  pinMode(pin_input, INPUT);
  pinMode(pin_frequency, INPUT);
  pinMode(pin_full_sram, OUTPUT);
  if (S.length() <= size_max) pause_send(false);
  sl_bit = 0;
  

}
int  Serial_make::available()
{
  if (S.length() > size_max) pause_send(true);
  return S.length();
}
char Serial_make::read()
{
  if (available() == 0) 
  {
    pause_send(false);
    return char(0);
  }
  char ch = S[0];
  //Serial.println(available());
  S.remove(0, 1);
  if (S.length() <= size_max) pause_send(false);
  return ch;


}
void Serial_make::receive_byte()
{
  int status = digitalRead(pin_input);

  if (status == HIGH){
      Bit[sl_bit++] = 1;
  }else Bit[sl_bit++] = 0;
  if (sl_bit == 8){
    sl_bit = 0;
    int ch = 0;
    for(int i= 0; i< 8; i++) ch += Bit[i]*(1<<i);
    S += char(ch);
  }
  if (S.length() > size_max) pause_send(true); 
}

void Serial_make::start_key()
{
    int status = digitalRead(pin_input);
    if (status == HIGH)
    {
      sl_bit = 0;
    }
}
void Serial_make::reset_stack()
{
  S = "";
}
int Serial_make::get_fre()
{
  
  return digitalPinToInterrupt(pin_frequency);
}
void Serial_make::change()
{
  if (digitalRead(pin_frequency) == HIGH) start_key();
  else receive_byte();
}
