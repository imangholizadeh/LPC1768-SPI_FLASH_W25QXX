#ifndef __FLASH_H
#define __FLASH_H
  
/*
���� ������ ������ � ������
�������� ����� ��� ���� ���� �����
W25Q
V=1.1 
by sinaset
centralclubs.com
Data:1395.12.09
*/

//#define SIZE_PAGE_W25Q          16384       //W25Q���� ����32
#define SIZE_PAGE_W25Q          32768       //W25Q���� ����64

#define SIZE                    256       //������ �� ���� ���� �����
#define W25Q_WriteEnable		0x06      //���� ���� ����� �����
#define W25Q_WriteDisable	  	0x04      //��� ���� ���� ����� �����
#define W25Q_ReadStatusReg		0x05      //������ ������ ����� 1
#define W25Q_ReadStatusReg2		0x35      //������ ������ ����� 1
#define W25Q_WriteStatusReg		0x01      //����� �� �����������
#define W25Q_ReadData		    0x03      //������ ����
#define W25Q_FastReadData	  	0x0B      //����� ���� ����
#define W25Q_FastReadDual	  	0x3B      //����� ����� ���ǡ�������
#define W25Q_PageProgram	  	0x02      //����� �� � ���� ����
#define W25Q_BlockErase32	    0x52      //�ǘ ���� � ��� ����
#define W25Q_BlockErase64		0xD8      //�ǘ ���� � ��� ����
#define W25Q_SectorErase		0x20     //�ǘ ���� � ��� ����
#define W25Q_ChipErase		  	0x60      //�ǘ ���� �� ��
#define W25Q_PowerDown			0xB9      //�� �� ����� �� ����
#define W25Q_ReleasePowerDown	0xAB      //
#define W25Q_ManufactDeviceID	0x90      //����� �� ������� � ����
#define FLASH_CS_HIGH() (LPC_GPIO0->FIOSET   |= (1<<16) )  //����� ��� ���� ���� ���� ����             
#define FLASH_CS_LOW() (LPC_GPIO0->FIOCLR   |= (1<<16) )  // ����� ���� ���� ���� ����� 

#pragma used+
unsigned char W25Q_read_StatustReg(unsigned char RG);           //������ ����� ��������
void W25Q_Write_StatusReg(unsigned char Data);                  //����� �� ������ �����
void wait_busy(void);                                            //������ ����� ��������
void W25Q_Write_Disable(void);                                   //��� ���� ���� ������ ����� �� ��� ����
void W25Q_Write(void);                                           //���� ���� ����� �� ��� ����
unsigned char W25Q_busy(void);                                   //������ ����� ��� ������
unsigned char W25Q_wel(void);                                    //������ ����� ��� �����

void W25Q_Page_Program(unsigned long int  Number_PAGE, unsigned char *Page_Data);        //����� �� � ���� ����
void W25Q_Page_Program_Res(unsigned int  *Number_PAGE_Res, unsigned char *Page_Data)  ; //����� �� ����� ����

void W25Q_Read_Data(unsigned long int  Number_PAGE,unsigned char *Page_Data);           //������ � ���� ����
void W25Q_Manufacturer_Device_ID(unsigned char *Manufacturer_id,unsigned char *Device_id);//��� ������� ������ ����
void W25Q_Sector_Erase(unsigned long int address );              //�ǘ ���� Ә������ 4 ���������
void W25Q_BlockErase_32(unsigned long int address );             //��� ��� ���� 32 ��������� �����
void W25Q_BlockErase_64(unsigned long int address );             //��� ��� ���� 64 ��������� �����
void W25Q_Chip_Erase(void );                                    //�ǘ ���� �� �����
void SPI_Flash_WAKEUP(void);                                    //����� ��� �� �� �� ���� ������
void SPI_Flash_PowerDown(void);                                 //���� �� �� �� ���� ������
void Factoty_rest_Reg(void);                                    //����� ��� ������� � ���� ������ 1
void Format_ic(char Type_format);                               //���� ���� �����
unsigned char Check_page(long int Page);                        //����� �� �� ���� ���� � ����
void Info_data_ic( unsigned  int *write, unsigned  int *no_write,unsigned  int *KByte_Used,char *Percent_Used,char Status_usart); 
//������� ����� �������
//void W25Q_Page_Program_Erase(unsigned long int  Number_PAGE, unsigned char *Page_Data); //������ �ǘ ���� � ����� �� ����


unsigned int spi(unsigned int val)
{
	

  /* Write and Read a byte on SPI interface. */
	unsigned int received = 0;
	 FLASH_CS_LOW();
  LPC_SPI->SPDR = val;
  while( !(LPC_SPI->SPSR & 0x80));//wait for transfer to be completed
   received=LPC_SPI->SPDR;
	FLASH_CS_HIGH() ;
   return(received);
}


unsigned char W25Q_read_StatustReg(unsigned char RG) //������ ����� ��������
{
unsigned char Data=0; 
   // W25Q_Write(); 
    FLASH_CS_LOW();
    if (RG==1)spi(W25Q_ReadStatusReg);   //������ ����� ������    
    if (RG==2)spi(W25Q_ReadStatusReg2);   //������ ����� ������ 
    Data= spi(0x00);
    spi(0x00);
    FLASH_CS_HIGH(); 
    return Data;                
}

void wait_busy(void) //������ ����� ��������
{
//while ((W25Q_read_StatustReg(1)&0x01)==0x01);
}
void W25Q_Write(void)     //���� ���� ����� �� ��� ����
{
FLASH_CS_LOW(); 
spi(W25Q_WriteEnable); 
FLASH_CS_HIGH();  
}


void W25Q_Write_Disable(void)  //��� ���� ���� ������ ����� �� ��� ����
{
FLASH_CS_LOW(); 
spi(W25Q_WriteDisable); 
FLASH_CS_HIGH();  
}

unsigned char W25Q_busy(void)    //������ ����� ��� ������
{
unsigned char Data=0;

    FLASH_CS_LOW(); 
    spi(W25Q_ReadStatusReg);   
    Data= spi(0x00);
    spi(0x00);
    FLASH_CS_HIGH();  
    if(Data & 0x80) Data=1;    
    else Data=0; 
    return Data;
}

unsigned char W25Q_wel(void)   //������ ����� ��� �����
{
unsigned char Data=0;

    FLASH_CS_LOW(); 
    spi(W25Q_ReadStatusReg);    
    Data= spi(0x00);
    spi(0x00);
    FLASH_CS_HIGH();  
    Data=Data<<1;
     if(Data & 0x80) Data=1;    
     else Data=0; 
     return Data;
}

void W25Q_Sector_Erase(unsigned long int address )   //�ǘ ���� Ә������ 4 ���������
{
unsigned char Data1,Data2,Data3;

//����� Ә��� ���� ��� 0 �� 1024 ����
//�� ���64 ����� Ә����� 2048 �� ����

//�� Ә��� 4 �������� �� ��� �� ���
address=address*SIZE*16;

Data1= (address>>16);
Data2= (address>>8);
Data3= (address);

   W25Q_Write(); 
   wait_busy();
   FLASH_CS_LOW(); 
   spi(W25Q_SectorErase);        //�ǘ ���� Ә���     
   spi(Data1); 
   spi(Data2);
   spi(Data3); 
   FLASH_CS_HIGH(); 
   wait_busy();                 
}

void W25Q_BlockErase_32(unsigned long int address )      //��� ��� ���� 32 ��������� �����
{
unsigned char Data1,Data2,Data3;

//����� ��� ���� ��� 0 �� 128 ����
//�� ��� 64 ����� 256 �� ����

//�� ��� ��� 64 �������� ��� �����
address=address*SIZE*128;
Data1= (address>>16);
Data2= (address>>8);
Data3= (address);
   W25Q_Write();
   FLASH_CS_LOW(); 
   spi(W25Q_BlockErase32);        //�ǘ ���� Ә���     
   spi(Data1); 
   spi(Data2);
   spi(Data3); 
   FLASH_CS_HIGH();
   wait_busy(); 
// delay_ms(900);                  
}

void W25Q_BlockErase_64(unsigned long int address )        //��� ��� ���� 64 ��������� �����
{
//����� ��� ���� ��� 0 �� 64 ����
//�� ��� 64 ����� 128 �� ����

//�� ��� ��� 64 �������� ��� �����
unsigned char Data1,Data2,Data3;

address=address*SIZE*256;
Data1= (address>>16);
Data2= (address>>8);
Data3= (address);
   W25Q_Write(); 
   wait_busy();
   FLASH_CS_LOW();  
   
   spi(W25Q_BlockErase64);        //�ǘ ���� Ә���     
   spi(Data1); 
   spi(Data2);
   spi(Data3); 
   
   FLASH_CS_HIGH(); 
   wait_busy();                   
}

void W25Q_Chip_Erase(void )     //�ǘ ���� �� �����
{
   W25Q_Write();  
   wait_busy();
   FLASH_CS_LOW();
   spi(W25Q_ChipErase);        //�ǘ ���� Ә���   
   FLASH_CS_HIGH(); 
   wait_busy();                  
}

void W25Q_Write_StatusReg(unsigned char Data)     //����� �� ������ �����
{
 
   W25Q_Write(); 
   FLASH_CS_LOW(); 
   spi(W25Q_WriteStatusReg);        //����� �����
   spi(Data); 
   spi(0x00);
   FLASH_CS_HIGH();  
}

void W25Q_Manufacturer_Device_ID(unsigned char *Manufacturer_id,unsigned char *Device_id)//��� ������� ������ ����
{ 
wait_busy();
    FLASH_CS_LOW(); 
    spi(W25Q_ManufactDeviceID);
    spi(0x00);
    spi(0x00);
    spi(0x00);
    *Manufacturer_id= spi(0x00);
    *Device_id= spi(0x00);
    FLASH_CS_HIGH(); 
    
  // delay_ms(10);    
}

void W25Q_Page_Program(unsigned long int  Number_PAGE, unsigned char *Page_Data)//����� �� � ���� ����
{                                                                             
//����� ���� ���� ��� 0 �� 16384 ����
//����� ���� ���� ����� ����� ����� �� 256 ���� ����
int g=0;
unsigned char Data1,Data2,Data3;

Number_PAGE=(Number_PAGE*256);      

Data1= (Number_PAGE>>16);
Data2= (Number_PAGE>>8);
Data3= (Number_PAGE);

    W25Q_Write();  
    FLASH_CS_LOW(); 
    spi(W25Q_PageProgram);        //�����
    spi(Data1);
    spi(Data2);
    spi(Data3); 
    for(g=0;g<SIZE;g++)spi(Page_Data[g]); 
    FLASH_CS_HIGH(); 
    //delay_us(750); 
     wait_busy(); 
}
/*
void W25Q_Page_Program_Erase(unsigned long int  Number_PAGE, unsigned char *Page_Data)//������ �ǘ ���� � ����� �� ����
{
//���� ������� �� ��� ���ڡ���� ��� ���� ��� ���� ����� Ә��� ����
//���� ��� ���� ������� ���� 1024 Ә��� 4 ���� ����� ���� �� �� �� ��� 16 Ә��� ���� ����
//���� ��� ���� ������� ��  ���ڡ�� ���� 0 ���� �� ���� ������� ����
unsigned int g,p;
unsigned char block=0,sector;
float M=0.0;
block=(Number_PAGE/256);
sector=(Number_PAGE/16);
M=(Number_PAGE/16)%10; 

if(M==1.00000 || M==2.00000 || M==3.00000 || M==4.00000 || M==5.00000 || M==6.00000 || M==7.00000 || M==8.00000 || M==9.00000 || M==0.00000 ){
W25Q_Sector_Erase(sector);  
}
W25Q_Page_Program(Number_PAGE,Page_Data);
} 
*/

void W25Q_Read_Data(unsigned long int  Number_PAGE,unsigned char *Page_Data)//������ � ���� ����
{
//����� ���� ���� ��� 0 �� 16384 ����
//����� ���� ���� ����� ����� ����� �� 256 ���� ����
int g=0;
unsigned char Data1,Data2,Data3;
Number_PAGE=(Number_PAGE*256);      
Data1= (Number_PAGE>>16);
Data2= (Number_PAGE>>8);
Data3= (Number_PAGE);

  //  W25Q_Write();  
    FLASH_CS_LOW(); 
    spi(W25Q_ReadData);       //������
    spi(Data1);
    spi(Data2);
    spi(Data3);   
    for(g=0;g<SIZE;g++)Page_Data[g]=spi(0x00);
    FLASH_CS_HIGH(); 
}

void SPI_Flash_PowerDown(void)  //�� ���� ���� 
{ 
  	FLASH_CS_LOW();                           
    spi(W25Q_PowerDown);      
	FLASH_CS_HIGH();                              	      
    //delay_us(3);                              
}   

void SPI_Flash_WAKEUP(void)      //����� ��� �� ����
{  
  	FLASH_CS_LOW();                          
    spi(W25Q_ReleasePowerDown);                                    	      
    FLASH_CS_HIGH();                                
} 

void Factoty_rest_Reg(void)        //���� ���� ������ ����� 1
{  
  W25Q_Write_StatusReg(0x00);                              
} ;

void Format_ic(char Type_format)     //���� ���� �� �����
{
//��� 0 ���� Ә��� �� Ә���
//��� 1 ���� ��ǘ ��� 32 ���������
//��� 2 ���� ��ǘ �� 64 ���������
int g;

  if(Type_format==0)for(g=0;g<1024;g++)W25Q_Sector_Erase(g);
  if(Type_format==1) for(g=0;g<128;g++)W25Q_BlockErase_32(g); 
  if(Type_format==2) for(g=0;g<64;g++)W25Q_BlockErase_64(g);
                                 
} ;

unsigned char Check_page(long int Page)     //����� �� �� ���� �� ����� ������� �� � ����
{

//�ѐ�� ��� 0 ���� ��� ���� ���� �� ���� � ���� �� ��� �� ����� ����
//�ѐ�� ��� 1 ���� �� ��� ���� ������ ������� ����� ��� ���
int g,c=0;
unsigned char Buff[256],DATA;
int No_write=0;

   W25Q_Read_Data(Page,Buff);

    for(g=0;g<256;g++)
    {
    DATA=Buff[g];
    if(DATA==0xff)c++;
    }
    
   if(c<250)No_write=1;                        
   else if(c>250)No_write=0;  
    
   return No_write ;                              
};

void W25Q_Page_Program_Res(unsigned int  *Number_PAGE_Res, unsigned char *Page_Data) 
//����� �� ����� ���� ���� ����
{
/*
�� �� ��� ���� �� ������ �� ����� ���� �����  ���  �� ��� � ���� ������� ���� � ���� �� ����� Ș���
���� ���� ����� ���� ����� ��� �� ����499 �� ����
�� �� �����
Page_Data
���� ��� �� ���� ������
���� ����� ���� �� ��� ����� ���� ����� ��� �� ���� �� ��� Ӂ� �� ���� ��� �� �� ���� �� �� ���� �� ���
���� ���� ����� ��� �� �� �����
Number_PAGE_Res
���� ����� �� ��� ��� �� ������  �� �� �� �� ����� ���� ������ ��� �������
�� ����� ���� �� �� ������ ���� ���� ���� �� ������ � ���� �� ������ �� ����

W25Q_Page_Program_Res(&ADDRESS,BUF)
���� ������
W25Q_Read_Data(ADDRESS,BUFF)
�� �� ������ ���� ���� ����� ��� �� �� ����� ��� ����� ���� � �� �� ����� �����
*/
unsigned int g,Check;
unsigned long int Address;

      for(g=0;g<SIZE_PAGE_W25Q;g++){
      Check=Check_page(g); 
      if(Check==0)Address=g,g=SIZE_PAGE_W25Q; 
      } 
      *Number_PAGE_Res=Address;
      W25Q_Page_Program(Address,Page_Data);

}


void Info_data_ic( unsigned  int *write, unsigned  int *no_write,unsigned  int *KByte_Used,char *Percent_Used,char Status_usart) 
//������� ����� �� �����
{
// ����� ��� ����� ���� ��� ����� ��� �� ���� �� ���
//����� ��� ����� ���� ��� ��� � ����� ���� �� ���� �� ���
//����� ��� ��� ������� ��� �� ���� �� И� �� ���
//����� ����� ��� ���� ������� �� �� ���� �� И� �� ���
unsigned  int g,c=0,WR_Y=0,WR_N=0,us=0;
char LD=0,LD2=0,str[15],str2[25];
int check=0;
    puts("Please Wait..\n\r"); 
    for(g=0;g<SIZE_PAGE_W25Q;g++)
    {
     check=Check_page(g);  
     if(check==1)WR_Y++;
     if(check==0)WR_N++; 
     
     
     us++;   
   //  if(Status_usart==1 && check==1)sprintf(str2,"NumberPage:%iwritten\n\r",g),puts(str2); 
     if(Status_usart==1 && us > (SIZE_PAGE_W25Q / 100))
       {
        us=0;
        LD++;
       // if(LD>10)LD=0,LD2+=10,sprintf(str,"Loading:%d\n\r",LD2),puts(str);
       }
    } 
    puts("Loading:100\n\r");
    *no_write=WR_N;
    *write=WR_Y;
    *KByte_Used=(WR_Y/4);
    if  (SIZE_PAGE_W25Q==16384)*Percent_Used= (WR_Y/4)/40; 
    else if (SIZE_PAGE_W25Q==32768)*Percent_Used= (WR_Y/4)/80;                          
};
#pragma used-

#endif
