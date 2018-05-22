const mysql = require('promise-mysql');			// mysql 모듈의 promise 버전

// rds 정보 입력 : hostname, username, password, default DB
const dbConfig = {
	host : 'mengfamily-rds-mysql.cnokttxubju1.ap-northeast-2.rds.amazonaws.com',
	port : 3306,
	user : 'root',
	password : '11111111',
	database : 'tetris_info',
	connectionLimit : 25
};

module.exports = mysql.createPool(dbConfig);	// connection pool을 module화