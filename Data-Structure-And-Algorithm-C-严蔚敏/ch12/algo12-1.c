*/! algo12-1.c ���������ļ������ظ������ļ����õ�һ���µ����ļ�  /
 #include"c1.h"
 typedef struct RedType /! ���ļ���¼���� */
 {
   int accounts; /* �ʺ� */
   int amount; /* ��� */
 }RedType; /* ���ļ���¼���� */
 typedef struct RcdType /* �����ļ���¼����(�����ļ���¼���Ͷ��˳�Աcode) */
 {
   int accounts; /* �ʺ� */
   int amount; /* ��ȡ������(��Ϊ+,ȡΪ-) */
   char code; /* �޸�Ҫ��(I:���� U:�޸� D:ɾ��) */
 }RcdType; /* �����ļ���¼����(�����ļ���¼���Ͷ��˳�Աcode) */
 #define key accounts
 RedType P(RcdType g)
 { /* ��g�ӹ�Ϊq�Ľṹ���� */
   RedType q;
   q.accounts=g.accounts;
   q.amount=g.amount;
   return q;
 }

 void Q(RedType *f,RcdType g)
 { /* ��f��g�鲢��һ��f�ṹ�ļ�¼ */
   (*f).amount+=g.amount;
 }

 void MergeFile(FILE *f,FILE *g,FILE *h)
 { /* �ɰ��ؼ��ֵ�������ķǿ�˳���ļ�f��g�鲢�õ����ļ�h�������ļ����� */
   /* �򿪣����У�f��gΪֻ���ļ����ļ��и�����һ�����ؼ��ּ�¼�� */
   /* ��g�ļ��жԸü�¼�Ĳ���Ϊ���롣hΪֻд�ļ����㷨12.1 */
   RedType fr,fn;
   RcdType gr;
   int i;
   fread(&fr,sizeof(RedType),1,f);
   fread(&gr,sizeof(RcdType),1,g);
   while(!feof(f)||!feof(g))
   {
     if(fr.key<gr.key)
       i=1;
     else if(gr.code=='D'&&fr.key==gr.key)
       i=2;
     else if(gr.code=='I'&&fr.key>gr.key)
       i=3;
     else if(gr.code=='U'&&fr.key==gr.key)
       i=4;
     else
       i=0;
     switch(i)
     {
       case 1: /* ���ơ��ɡ����ļ��м�¼ */
      fwrite(&fr,sizeof(RedType),1,h);
      if(!feof(f))
        fread(&fr,sizeof(RedType),1,f);
       break;
       case 2: /* ɾ�����ɡ����ļ��м�¼���������� */
               if(!feof(f))
                 fread(&fr,sizeof(RedType),1,f);
               if(!feof(g))
                 fread(&gr,sizeof(RcdType),1,g);
               break;
       case 3: /* ���� */
               fn=P(gr); /* ����P��gr�ӹ�Ϊh�Ľṹ */
               fwrite(&fn,sizeof(RedType),1,h);
               if(!feof(g))
	            fread(&gr,sizeof(RcdType),1,g);
               break;
       case 4: /* ���ġ��ɡ����ļ��м�¼ */
               Q(&fr,gr); /* ����Q��fr��gr�鲢��һ��h�ṹ�ļ�¼ */
               fwrite(&fr,sizeof(RedType),1,h);
               if(!feof(f))
	            fread(&fr,sizeof(RedType),1,f);
               if(!feof(g))
	            fread(&gr,sizeof(RcdType),1,g);
               break;
       default:exit(ERROR); /* ������Ϊ������� */
     }
   }
 }

 void print(RedType t)
 {
   printf("%6d%4d\n",t.accounts,t.amount);
 }

 void printc(RcdType t)
 {
   printf("%6d%6d%8c\n",t.accounts,t.amount,t.code);
 }

 void main()
 {
   RedType c,a[8]={{1,50},{5,78},{12,100},{14,95},{15,360},{18,200},{20,510},{INT_MAX,0}}; /* ���ļ����� */
   RcdType d,b[6]={{8,100,'I'},{12,-25,'U'},{14,38,'U'},{18,-200,'D'},{21,60,'I'},{INT_MAX,0,'U'}}; /* ������������ļ����� */
   FILE *f,*g,*h;
   int j;
   f=fopen("old","wb"); /* ��д�ķ�ʽ�����ļ�old */
   fwrite(a,sizeof(RedType),8,f); /* ������a�е�����д���ļ�old */
   fclose(f); /* �ر��ļ�old���γ����ļ� */
   f=fopen("change","wb"); /* ��д�ķ�ʽ�������ļ�change */
   fwrite(b,sizeof(RcdType),6,f); /* ������b�е�����д���ļ�change */
   fclose(f); /* �ر��ļ�change���γ�������������ļ� */
   f=fopen("old","rb"); /* �Զ��ķ�ʽ�����ļ�old */
   printf("���ļ�����:\n");
   printf("  �ʺ� ���\n");
   do
   {
     j=fread(&c,sizeof(RedType),1,f);
     if(j==1)
       print(c); /* ���r������ */
   }while(j==1);
   rewind(f); /* ʹf��ָ�����·����ļ�����ʼλ�ã��Ա����¶����ڴ� */
   g=fopen("change","rb"); /* �Զ��ķ�ʽ��������������ļ�change */
   printf("������������ļ�����:\n");
   printf("  �ʺ� ��ȡ���� �޸�Ҫ��\n");
   do
   {
     j=fread(&d,sizeof(RcdType),1,g);
     if(j==1)
       printc(d); /* ���r������ */
   }while(j==1);
   rewind(g); /* ʹg��ָ�����·����ļ�����ʼλ�ã��Ա����¶����ڴ� */
   h=fopen("new","wb"); /* ��д�ķ�ʽ�������ļ�new */
   MergeFile(f,g,h); /* ���������ļ� */
   fclose(f); /* �ر��ļ�old */
   fclose(g); /* �ر��ļ�change */
   fclose(h); /* �ر��ļ�new */
   f=fopen("new","rb"); /* �Զ��ķ�ʽ�������ļ�new */
   printf("�����ļ�����:\n");
   printf("  �ʺ� ���\n");
   do
   {
     j=fread(&c,sizeof(RedType),1,f);
     if(j==1)
       print(c); /* ���r������ */
   }while(j==1);
   fclose(f); /* �ر��ļ�new */
 }
 