CREATE TABLE Table1 (ID INT PRIMARY KEY NOT NULL,
                    Value INT);

create sequence IDTable1 Increment by 1 start with 1;
insert into table1(id,value) values (IDTable1.nextval,35);
insert into table1(id,value) values (IDTable1.nextval,35);
insert into table1(id,value) values (IDTable1.nextval,42);
insert into table1(id,value) values (IDTable1.nextval,53);
insert into table1(id,value) values (IDTable1.nextval,66);
insert into table1(id,value) values (IDTable1.nextval,12);
insert into table1(id,value) values (IDTable1.nextval,23);

CREATE TABLE Table2 (ID INT PRIMARY KEY NOT NULL,
                    Value INT);

create sequence IDTable2 Increment by 1 start with 1;
insert into table2(id,value) values (IDTable2.nextval,25);
insert into table2(id,value) values (IDTable2.nextval,24);
insert into table2(id,value) values (IDTable2.nextval,73);
insert into table2(id,value) values (IDTable2.nextval,66);
insert into table2(id,value) values (IDTable2.nextval,35);
insert into table2(id,value) values (IDTable2.nextval,42);
insert into table2(id,value) values (IDTable2.nextval,12);
