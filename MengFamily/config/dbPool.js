const mysql = require('promise-mysql');			// mysql 모듈의 promise 버전

// rds 정보 입력 : hostname, username, password, default DB
const dbConfig = {
	host : 'hostname',
	port : 3306,
	user : 'username',
	password : 'password',
	database : 'defaultDB',
	connectionLimit : 20
};

module.exports = mysql.createPool(dbConfig);	// connection pool을 module화