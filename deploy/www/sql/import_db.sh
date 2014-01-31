#!/bin/bash

clear

mysql -p -u coupier coupier < /srv/www/poker/root/htdocs/sql/drop_tables.sql
mysql -p -u coupier coupier < /srv/www/poker/root/htdocs/sql/croupierweb.sql

echo "Imported."
