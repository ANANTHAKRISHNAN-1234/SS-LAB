#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void display()
{
    char ch;
    FILE *fp1,*fp4,*fp3;
    printf("The contents of intermediate files are:\n\n");
    fp3=fopen("intermediate.txt","r");
    ch=fgetc(fp3);
    while(ch!=EOF)
    {
        printf("%c",ch);
       ch=fgetc(fp3); 
    }
    fclose(fp3);
     printf("The contents of output files are:\n\n");
     fp1=fopen("output.txt","r");
    ch=fgetc(fp1);
    while(ch!=EOF)
    {
        printf("%c",ch);
       ch=fgetc(fp1); 
    }
    fclose(fp1);
     printf("The object code is:\n\n");
    fp4=fopen("objcode.txt","r");
    ch=fgetc(fp4);
    while(ch!=EOF)
    {
        printf("%c",ch);
       ch=fgetc(fp4); 
    }
    fclose(fp4);
}
int main()
{
    char a[20],ad[20],label[20],opcode[20],operand[20],symbol[20];
    int prevaddr,address,diff,finaddr,start,addr,actual_len,len,i,j=0;
    char code[20][20]={"LDA","STA","LDCH","STCH"};
    char mnemonic[20][20]={"33","44","57","64"};
    FILE *fp1,*fp2,*fp3,*fp4;
    fp1=fopen("output.txt","w");
    fp2=fopen("symtab.txt","r");
    fp3=fopen("intermediate.txt","r");
    fp4=fopen("objcode.txt","w");
    fscanf(fp3,"%s\t%s\t%s",label,opcode,operand);
    while(strcmp(opcode,"END")!=0)
    {
        prevaddr=address;
        fscanf(fp3,"%d\t%s\t%s\t%s",&address,label,opcode,operand);
    }
    finaddr=address;
    fclose(fp3);
    fp3=fopen("intermediate.txt","r");
    fscanf(fp3,"%s\t%s\t%s",label,opcode,operand);
    if(strcmp(opcode,"START")==0)
    {
      fprintf(fp1,"\t%s\t%s\t%s\n",label,opcode,operand);
        fprintf(fp4,"H^%s^%s^%d\n",label,operand,finaddr);
        fscanf(fp3,"%d\t%s\t%s\t%s",&address,label,opcode,operand);
        start=address;
        diff=prevaddr-start;
        fprintf(fp4,"T^%d^%d",start,diff);
    }
    while(strcmp(opcode,"END")!=0)
    {
         if(strcmp(opcode,"BYTE")==0)
         { 
            fprintf(fp1,"%d\t%s\t%s\t%s\t",address,label,opcode,operand);
             len=strlen(operand);
            actual_len=len-3;
            fprintf(fp4,"^");
            for(i=2;i<(actual_len+2);i++)
            {
                sprintf(ad,"%02X",operand[i]);
                fprintf(fp1,"%s",ad);
                fprintf(fp4,"%s",ad);
            }
            fprintf(fp1,"\n");
         }
         else if(strcmp(opcode,"WORD")==0)
         {
             sprintf(a,"%06X",atoi(operand));
             fprintf(fp1,"%d\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,a);
             fprintf(fp4,"^%s",a);
         }
         else if((strcmp(opcode,"RESW")==0)||strcmp(opcode,"RESB")==0)
         { 
             fprintf(fp1,"%d\t%s\t%s\t%s\n",address,label,opcode,operand);
         }
         else{
              while(strcmp(opcode,code[j])!=0)
              {
                j++;
              }
              rewind(fp2);
              fscanf(fp2,"%s\t%d",symbol,&addr);
              while(strcmp(operand,symbol)!=0)
              {
                 fscanf(fp2,"%s\t%d\n",symbol,&addr);
              }
             fprintf(fp1,"%d\t%s\t%s\t%s\t%s%d\n",address,label,opcode,operand,mnemonic[j],addr);
             fprintf(fp4,"^%s%d",mnemonic[j],addr);
         }
         fscanf(fp3,"%d\t%s\t%s\t%s",&address,label,opcode,operand);
    }
    fprintf(fp1,"%d\t%s\t%s\t%s\n",address,label,opcode,operand);
    fprintf(fp4,"\nE^%d",start);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    display();
    return 0;
}