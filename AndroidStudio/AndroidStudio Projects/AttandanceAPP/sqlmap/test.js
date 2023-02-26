const fs = require("fs");
const mysql = require("mysql");
const express = require("express");

const app = express();

//port
const PORT = 3000;

//middleware
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// 불러온 database.json을 파싱하여 conf 객체에 넣습니다.
const data = fs.readFileSync("./DBconfig.json");
const conf = JSON.parse(data);

// database.json에 설정한 값을 넣어 연결을 생성합니다.
const connection = mysql.createConnection({
  host: conf.host,
  user: conf.user,
  password: conf.password,
  port: conf.port,
  database: conf.database,
});

// 연결을 수행합니다.
connection.connect();

// 해당 라우터로 접속하면 SQL을 날리고 그 결과물을 전송합니다.
app.get("/departmentinfo", (req, res) => {
  sql = "SELECT * FROM DEPARTMENT_INFO";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
  });
});

app.get("/holiday", (req, res) => {
  sql = "SELECT * FROM CALENDAR";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
  });
});

app.get("/userinfo/:company_id", (req, res) => {
  let { company_id } = req.params;
  company_id = company_id.replace(":", "");
  console.log("userinfo: ", company_id);
  sql = "SELECT user_id, passwd, user_type FROM USER_INFO WHERE company_id='"+company_id+"'";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
  });
});

app.get("/userallinfo/:user_id", (req, res) => {
  let { user_id} = req.params;
  user_id= user_id.replace(":", "");
  sql = "SELECT ui.*, di.* ";
  sql += "FROM USER_INFO ui, DEPARTMENT_INFO di ";
  sql += "WHERE ui.user_id = '" + user_id + "' ";
  sql += "AND	ui.department_id = di.department_id ";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
  });
});

app.get("/alluser", (req, res) => {
  sql = "SELECT user_id FROM USER_INFO";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
  });
});

app.get("/calendar", (req, res) => {
  sql = "SELECT * FROM CALENDAR";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
  });
});

app.get("/usernow/:user_id/:currentDate", (req, res) => {
  let { user_id } = req.params;
  let { currentDate } = req.params;
  user_id = user_id.replace(":", "");
  currentDate = currentDate.replace(":", "");
  console.log("usernow: ", user_id, currentDate);
  sql = "SELECT us.*, uas.setted_attend_time, uas.setted_off_time, IFNULL((SELECT over_type FROM OVER_WORK WHERE user_id='"+user_id+"' AND attend_date LIKE '%"+currentDate+"%'),-1) AS over_type FROM USER_STATE us, USER_ATTEND_SETTING uas WHERE us.user_id='"+user_id+"' AND us.attend_date LIKE '%"+currentDate+"%' AND us.user_id=uas.user_id";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
  });
});

app.get("/nowoverwork/:user_id/:currentDate", (req, res) => {
  let { user_id } = req.params;
  let { currentDate } = req.params;
  user_id = user_id.replace(":", "");
  currentDate = currentDate.replace(":", "");
  console.log("overwork: ", user_id, currentDate);
  sql = "SELECT over_type FROM OVER_WORK WHERE user_id='"+user_id+"' AND attend_date LIKE '%"+currentDate+"%' AND over_permit=1";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
  });
});

app.get("/userattendsetting/:user_id", (req, res) => {
  let { user_id } = req.params;
  user_id=user_id.replace(":", "");
  console.log("userattendsetting: ", user_id);
  sql = "SELECT * FROM USER_ATTEND_SETTING WHERE user_id='" + user_id +"'";
  connection.query(sql, (err, rows, fields) => {
    console.log(sql);
    res.send(rows);
    console.log(rows);
  });
});

app.get("/userstate/:user_id", (req, res) => {
	let { user_id } = req.params;
	user_id=user_id.replace(":", "");
	sql = "SELECT us.*, uas.*,"
	sql += "IFNULL((SELECT outside_type FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date=us.attend_date AND outside_permit=1), -1) AS outside_type,";
	sql += "IFNULL((SELECT outside_x FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date=us.attend_date AND outside_permit=1), -1) AS outside_x,";
	sql += "IFNULL((SELECT outside_y FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date=us.attend_date AND outside_permit=1), -1) AS outside_y,";
	sql += "IFNULL((SELECT outside_start_time FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date=us.attend_date AND outside_permit=1), -1) AS outside_start_time,";
	sql += "IFNULL((SELECT outside_end_time FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date=us.attend_date AND outside_permit=1), -1) AS outside_end_time,";
	sql += "IFNULL((SELECT over_type FROM OVER_WORK ow WHERE uas.user_id=user_id AND ow.attend_date=us.attend_date AND over_permit=1), -1) AS over_type,";
	sql += "IFNULL((SELECT over_start_time FROM OVER_WORK ow WHERE uas.user_id=user_id AND ow.attend_date=us.attend_date AND over_permit=1), -1) AS over_start_time,";
    sql += "IFNULL((SELECT over_end_time FROM OVER_WORK ow WHERE uas.user_id=user_id AND ow.attend_date=us.attend_date AND over_permit=1), -1) AS over_end_time,";
	sql += "IFNULL((SELECT business_permit FROM BUSINESS_TRIP WHERE uas.user_id=user_id AND (business_start_date>=us.attend_date AND business_end_date<=us.attend_date) AND business_permit=1), -1) AS business_type,";
	sql += "IFNULL((SELECT business_x FROM BUSINESS_TRIP WHERE uas.user_id=user_id AND (business_start_date>=us.attend_date AND business_end_date<=us.attend_date) AND business_permit=1), -1) AS business_x,";
	sql += "IFNULL((SELECT business_y FROM BUSINESS_TRIP WHERE uas.user_id=user_id AND (business_start_date>=us.attend_date AND business_end_date<=us.attend_date) AND business_permit=1), -1) AS business_y,";
	sql += "IFNULL((SELECT vacation_type FROM VACATION WHERE uas.user_id=user_id AND (vacation_start_date>=us.attend_date AND vacation_end_date<=us.attend_date) AND vacation_permit=1), -1) AS vacation_type ";
	sql += "FROM USER_STATE us, USER_ATTEND_SETTING uas WHERE us.user_id='"+user_id+"' AND us.user_id=uas.user_id ORDER BY attend_date ASC";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/attendoffnow/:user_id/:date", (req, res) => {
  var params = req.params;
  var user_id = params.user_id.replace(":", "");
  var date = params.date.replace(":", "");
  sql = "SELECT uas.*, ui.*,";
  sql += "IFNULL((SELECT attend_state FROM USER_STATE WHERE uas.user_id=user_id AND attend_date='"+date+"'), -1) AS attend_state,";
  sql += "IFNULL((SELECT attend_time FROM USER_STATE WHERE uas.user_id=user_id AND attend_date='"+date+"'), -1) AS attend_time,";
  sql += "IFNULL((SELECT off_time FROM USER_STATE WHERE uas.user_id=user_id AND attend_date='"+date+"'), -1) AS off_time,";
  sql += "IFNULL((SELECT outside_type FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date='"+date+"' AND outside_permit=1), -1) AS outside_type,";
  sql += "IFNULL((SELECT outside_x FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date='"+date+"' AND outside_permit=1), -1) AS outside_x,";
  sql += "IFNULL((SELECT outside_y FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date='"+date+"' AND outside_permit=1), -1) AS outside_y,";
  sql += "IFNULL((SELECT outside_start_time FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date='"+date+"' AND outside_permit=1), -1) AS outside_start_time,";
  sql += "IFNULL((SELECT outside_end_time FROM OUTSIDE WHERE uas.user_id=user_id AND outside_date='"+date+"' AND outside_permit=1), -1) AS outside_end_time,";
  sql += "IFNULL((SELECT business_permit FROM BUSINESS_TRIP WHERE uas.user_id=user_id AND (business_start_date>='"+date+"' AND business_end_date<='"+date+"') AND business_permit=1), -1) AS business_type,";
  sql += "IFNULL((SELECT business_x FROM BUSINESS_TRIP WHERE uas.user_id=user_id AND (business_start_date>='"+date+"' AND business_end_date<='"+date+"') AND business_permit=1), -1) AS business_x,";
  sql += "IFNULL((SELECT business_y FROM BUSINESS_TRIP WHERE uas.user_id=user_id AND (business_start_date>='"+date+"' AND business_end_date<='"+date+"') AND business_permit=1), -1) AS business_y,";
  sql += "IFNULL((SELECT vacation_type FROM VACATION WHERE uas.user_id=user_id AND (vacation_start_date>='"+date+"' AND vacation_end_date<='"+date+"') AND vacation_permit=1), -1) AS vacation_type, ";
  sql += "IFNULL((SELECT over_type FROM OVER_WORK WHERE uas.user_id=user_id AND attend_date='"+date+"' AND over_permit=1), -1) AS over_type, ";
  sql += "IFNULL((SELECT over_x FROM OVER_WORK WHERE uas.user_id=user_id AND attend_date='"+date+"' AND over_permit=1), -1) AS over_x, ";
  sql += "IFNULL((SELECT over_y FROM OVER_WORK WHERE uas.user_id=user_id AND attend_date='"+date+"' AND over_permit=1), -1) AS over_y, ";
  sql += "IFNULL((SELECT over_start_time FROM OVER_WORK WHERE uas.user_id=user_id AND attend_date='"+date+"' AND over_permit=1), -1) AS over_start_time, ";
  sql += "IFNULL((SELECT over_end_time FROM OVER_WORK WHERE uas.user_id=user_id AND attend_date='"+date+"' AND over_permit=1), -1) AS over_end_time, ";
  sql += "IFNULL((SELECT name FROM CALENDAR WHERE holiday='"+date+"'), '-1') AS holiday_name ";
  sql += "FROM USER_ATTEND_SETTING uas, USER_INFO ui WHERE ui.user_id = '"+user_id+"'AND uas.user_id = ui.user_id";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
    console.log(rows);
  });
});

app.get("/insertattend/:user_id/:attend_state/:attend_time/:attend_day/:attend_date/:work_type/:attend_x/:attend_y", (req, res) => {
  var params = req.params;
  params.user_id = params.user_id.replace(":", "");
  params.attend_state = parseInt(params.attend_state.replace(":", ""));
  params.attend_time = params.attend_time.replace(":", "");
  params.attend_day = params.attend_day.replace(":", "");
  params.attend_date = params.attend_date.replace(":", "");
  params.work_type = parseInt(params.work_type.replace(":", ""));
  params.attend_x = parseFloat(params.attend_x.replace(":", ""));
  params.attend_y = parseFloat(params.attend_y.replace(":", ""));
  console.log(params);
  sql = "INSERT INTO USER_STATE(user_id, attend_state, attend_time, attend_day, attend_date, work_type, attend_x, attend_y) VALUES('"+params.user_id+"',"+params.attend_state+",'"+params.attend_time+"','"+params.attend_day+"','"+params.attend_date+"',"+params.work_type+","+params.attend_x+","+params.attend_y+")";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
    console.log(rows);
  });
});

app.get("/updateoff/:user_id/:attend_state/:off_time/:attend_date/:per_day/:over_time/:work_type/:off_x/:off_y", (req, res) => {
  var params = req.params;
  params.user_id = params.user_id.replace(":", "");
  params.attend_state = parseInt(params.attend_state.replace(":", ""));
  params.off_time = params.off_time.replace(":", "");
  params.attend_date = params.attend_date.replace(":", "");
  params.per_day = parseInt(params.per_day.replace(":", ""));
  params.over_time = parseInt(params.over_time.replace(":", ""));
  params.work_type = parseInt(params.work_type.replace(":", ""));
  params.off_x = parseFloat(params.off_x.replace(":", ""));
  params.off_y = parseFloat(params.off_y.replace(":", ""));
  console.log(params);
  sql = "UPDATE USER_STATE SET attend_state="+params.attend_state+", off_time='"+params.off_time+"', work_time_per_day="+params.per_day+", over_time="+params.over_time+", work_type="+params.work_type+", off_x="+params.off_x+", off_y="+params.off_y+" WHERE user_id='"+params.user_id+"' AND attend_date='"+params.attend_date+"'";
  connection.query(sql, (err, rows, fields) => {
    res.send(rows);
    console.log(rows);
  });
});
app.get("/vacationall", (req, res) => {
	sql =	"SELECT ui.user_nm, ui.employee, ui.join_date, di.department_nm, uvi.* ";
	sql +=	"FROM USER_INFO ui, DEPARTMENT_INFO di, USER_VACATION_INFO uvi ";
	sql +=	"WHERE uvi.user_id = ui.user_id ";
	sql +=	"AND ui.department_id = di.department_id ";
	sql +=	"ORDER BY ui.user_nm ASC ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/vacationinfo/:user_id", (req, res) => {
	let { user_id } = req.params;
	user_id=user_id.replace(":", "");
	sql =	"SELECT ui.user_nm, ui.employee, ui.join_date, di.department_nm, uvi.* ";
	sql +=	"FROM USER_INFO ui, DEPARTMENT_INFO di, USER_VACATION_INFO uvi ";
	sql +=	"WHERE ui.user_id = '"+user_id+"' ";
	sql +=	"AND uvi.user_id = ui.user_id ";
	sql +=	"AND ui.department_id = di.department_id ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/vacationstate/:user_id", (req, res) => {
	let { user_id } = req.params;
	user_id=user_id.replace(":", "");
	sql =	"SELECT @ROWNUM:=@ROWNUM+1 AS row_num, v.* ";
	sql +=	"FROM VACATION v, (SELECT @ROWNUM:=0) AS r ";
	sql +=	"WHERE v.user_id = '"+user_id+"' ";
	sql +=	"AND v.vacation_permit=1 ";
	sql +=	"ORDER BY v.vacation_start_date ASC";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/vacationselect/:user_id", (req, res) => {
	let { user_id } = req.params;
	user_id=user_id.replace(":", "");
	sql =	"SELECT tmp.*, ui.user_nm AS permit_nm ";
	sql +=	"FROM (SELECT v.*, u.user_nm FROM VACATION v, USER_INFO u WHERE u.user_id=v.user_id AND u.user_id='"+user_id+"' ";
	sql +=	"ORDER BY v.vacation_start_date DESC) tmp, USER_INFO ui ";
	sql +=	"WHERE IFNULL(ui.user_id = tmp.permit_id, ui.user_id='0') ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/businessselect/:user_id", (req, res) => {
	let { user_id } = req.params;
	user_id=user_id.replace(":", "");
	sql =	"SELECT tmp.*, ui.user_nm AS permit_nm ";
	sql +=	"FROM (SELECT b.*, u.user_nm FROM BUSINESS_TRIP b, USER_INFO u WHERE u.user_id=b.user_id AND u.user_id='"+user_id+"' ";
	sql +=	"ORDER BY b.business_start_date DESC) tmp, USER_INFO ui ";
	sql +=	"WHERE IFNULL(ui.user_id = tmp.permit_id, ui.user_id='0') ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/outsideselect/:user_id", (req, res) => {
	let { user_id } = req.params;
	user_id=user_id.replace(":", "");
	sql =	"SELECT tmp.*, ui.user_nm AS permit_nm ";
	sql +=	"FROM (SELECT o.*, u.user_nm FROM OUTSIDE o, USER_INFO u WHERE u.user_id=o.user_id AND u.user_id='"+user_id+"' ";
	sql +=	"ORDER BY o.outside_date DESC) tmp, USER_INFO ui ";
	sql +=	"WHERE IFNULL(ui.user_id = tmp.permit_id, ui.user_id='0') ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/overworkselect/:user_id", (req, res) => {
	let { user_id } = req.params;
	user_id=user_id.replace(":", "");
	sql =	"SELECT tmp.*, ui.user_nm AS permit_nm ";
	sql +=	"FROM (SELECT o.*, u.user_nm FROM OVER_WORK o, USER_INFO u WHERE u.user_id=o.user_id AND u.user_id='"+user_id+"' ";
	sql +=	"ORDER BY o.attend_date DESC) tmp, USER_INFO ui ";
	sql +=	"WHERE IFNULL(ui.user_id = tmp.permit_id, ui.user_id='0') ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/recognizeselectvacation", (req, res) => {
	sql =	"SELECT tmp.*, ui.user_nm AS permit_nm ";
	sql +=	"FROM (SELECT v.*, u.user_nm FROM VACATION v, USER_INFO u WHERE vacation_permit=0 AND u.user_id=v.user_id ";
	sql +=	"ORDER BY v.vacation_start_date DESC) tmp, USER_INFO ui ";
	sql +=	"WHERE IFNULL(ui.user_id = tmp.permit_id, ui.user_id='0') ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/recognizeupdatevacation/:num/:vacation_permit/:permit_date/:permit_id/:year/:month/:replace", (req, res) => {
  	var params = req.params;
  	params.num = params.num.replace(":", "");
  	params.vacation_permit = parseInt(params.vacation_permit.replace(":", ""));
  	params.permit_date = params.permit_date.replace(":", "");
	params.permit_id = params.permit_id.replace(":", "");
	params.year = parseInt(params.year.replace(":", ""));
    params.month = parseInt(params.month.replace(":", ""));
	params.replace = parseInt(params.replace.replace(":", ""));
	sql =	"UPDATE VACATION SET ";
	sql +=	"vacation_permit="+params.vacation_permit+", permit_date='"+params.permit_date+"', permit_id='"+params.permit_id+"', ";
	sql += "now_use_year="+params.year+", now_use_month="+params.month+", now_use_replace="+params.replace + " ";
	sql +=	"WHERE num="+params.num+" ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
		console.log(err);
	});
});

app.get("/vacationupdate/:user_id/:get_year/:year/:month/:replace/:remaining", (req, res) => {
  	var params = req.params;
  	params.user_id = params.user_id.replace(":", "");
  	params.get_year = params.get_year.replace(":", "");
	params.year = parseInt(params.year.replace(":", ""));
    params.month = parseInt(params.month.replace(":", ""));
	params.replace = parseInt(params.replace.replace(":", ""));
	params.remaining = parseInt(params.remaining.replace(":", ""));
	sql =	"UPDATE USER_VACATION_INFO SET ";
	sql +=	"use_vacation_year="+params.year+", use_vacation_month="+params.month+", use_vacation_replace="+params.replace+", ";
	sql += " remaining_day="+params.remaining + " ";
	sql +=	"WHERE user_id='"+params.user_id+"' AND get_year='"+params.get_year+"' ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
		console.log(err);
	});
});

app.get("/recognizeselectbusiness", (req, res) => {
	sql =	"SELECT tmp.*, ui.user_nm AS permit_nm ";
	sql +=	"FROM (SELECT b.*, u.user_nm FROM BUSINESS_TRIP b, USER_INFO u WHERE business_permit=0 AND u.user_id=b.user_id ";
	sql +=	"ORDER BY b.business_start_date DESC) tmp, USER_INFO ui ";
	sql +=	"WHERE IFNULL(ui.user_id = tmp.permit_id, ui.user_id='0') ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/recognizeupdatebusiness/:num/:business_permit/:permit_date/:permit_id", (req, res) => {
  	var params = req.params;
  	params.num = params.num.replace(":", "");
  	params.business_permit = parseInt(params.business_permit.replace(":", ""));
  	params.permit_date = params.permit_date.replace(":", "");
	params.permit_id = params.permit_id.replace(":", "");
	sql =	"UPDATE BUSINESS_TRIP SET ";
	sql +=	"business_permit="+params.business_permit+", permit_date='"+params.permit_date+"', permit_id='"+params.permit_id+"' ";
	sql +=	"WHERE num="+params.num+" ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/recognizeselectoutside", (req, res) => {
	sql =	"SELECT tmp.*, ui.user_nm AS permit_nm ";
	sql +=	"FROM (SELECT o.*, u.user_nm FROM OUTSIDE o, USER_INFO u WHERE outside_permit=0 AND u.user_id=o.user_id ";
	sql +=	"ORDER BY o.outside_date DESC) tmp, USER_INFO ui ";
	sql +=	"WHERE IFNULL(ui.user_id = tmp.permit_id, ui.user_id='0') ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/recognizeupdateoutside/:num/:outside_permit/:permit_date/:permit_id", (req, res) => {
  	var params = req.params;
  	params.num = params.num.replace(":", "");
  	params.outside_permit = parseInt(params.outside_permit.replace(":", ""));
  	params.permit_date = params.permit_date.replace(":", "");
	params.permit_id = params.permit_id.replace(":", "");
	sql =	"UPDATE OUTSIDE SET ";
	sql +=	"outside_permit="+params.outside_permit+", permit_date='"+params.permit_date+"', permit_id='"+params.permit_id+"' ";
	sql +=	"WHERE num="+params.num+" ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/recognizeselectover", (req, res) => {
	sql =	"SELECT tmp.*, ui.user_nm AS permit_nm ";
	sql +=	"FROM (SELECT o.*, u.user_nm FROM OVER_WORK o, USER_INFO u WHERE over_permit=0 AND u.user_id=o.user_id ";
	sql +=	"ORDER BY o.attend_date DESC) tmp, USER_INFO ui ";
	sql +=	"WHERE IFNULL(ui.user_id = tmp.permit_id, ui.user_id='0') ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/recognizeupdateover/:num/:over_permit/:permit_date/:permit_id", (req, res) => {
  	var params = req.params;
  	params.num = params.num.replace(":", "");
  	params.over_permit = parseInt(params.over_permit.replace(":", ""));
  	params.permit_date = params.permit_date.replace(":", "");
	params.permit_id = params.permit_id.replace(":", "");
	sql =	"UPDATE OVER_WORK SET ";
	sql +=	"over_permit="+params.over_permit+", permit_date='"+params.permit_date+"', permit_id='"+params.permit_id+"' ";
	sql +=	"WHERE num="+params.num+" ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});

app.get("/insertalert/:user_id/:type/:start_date/:period/:status", (req, res) => {
  var params = req.params;
  params.user_id = params.user_id.replace(":", "");
  params.type = params.type.replace(":", "");
  params.start_date = params.start_date.replace(":", "");
  params.period = parseInt(params.period.replace(":", ""));
  params.status = params.status.replace(":", "");
  console.log(params);
  sql = "INSERT INTO ALERT(user_id, type, start_date, period, status) VALUES('"+params.user_id+"', '"+params.type+"', '"+params.start_date+"', "+params.period+", '"+params.status+"')";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
		console.log(err);
	});
});

app.get("/insertvacation/:user_id/:type/:draft/:start/:end/:period/:reason/:year/:use_year/:month/:use_month/:replace/:use_replace", (req, res) => {
  var params = req.params;
  params.user_id = params.user_id.replace(":", "");
  params.type =parseInt(params.type.replace(":", ""));
  params.draft = params.draft.replace(":", "");
  params.start = params.start.replace(":", "");
  params.end = params.end.replace(":", "");
  params.period = parseInt(params.period.replace(":", ""));
  params.reason = params.reason.replace(":", "");
  params.year= parseInt(params.year.replace(":", ""));
  params.use_year= parseInt(params.use_year.replace(":", ""));
  params.month= parseInt(params.month.replace(":", ""));
  params.use_month= parseInt(params.use_month.replace(":", ""));
  params.replace= parseInt(params.replace.replace(":", ""));
  params.use_replace= parseInt(params.use_replace.replace(":", ""));
  console.log(params);
  sql = "INSERT INTO VACATION(user_id, vacation_type, draft_date, vacation_start_date, vacation_end_date, vacation_period, vacation_reason, ";
  sql+="now_year, now_use_year, now_month, now_use_month, now_replace, now_use_replace) VALUES('"+params.user_id+"', "+params.type+", '";
  sql+=params.draft+"', '"+params.start+"', '"+params.end+"', "+params.period+", '"+params.reason+"', "+ params.year+", "+ params.use_year+", ";
  sql+=params.month + ", "+params.use_month+ ", "+params.replace+", "+ params.use_replace+")";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
		console.log(err);
	});
});

app.get("/insertbusiness/:user_id/:draft/:start/:end/:period/:reason/:x/:y/:address", (req, res) => {
  var params = req.params;
  params.user_id = params.user_id.replace(":", "");
  params.draft = params.draft.replace(":", "");
  params.start = params.start.replace(":", "");
  params.end = params.end.replace(":", "");
  params.period = parseInt(params.period.replace(":", ""));
  params.reason = params.reason.replace(":", "");
  params.x= parseFloat(params.x.replace(":", ""));
  params.y= parseFloat(params.y.replace(":", ""));
  params.address= params.address.replace(":", "");
  console.log(params);
  sql = "INSERT INTO BUSINESS_TRIP(user_id, draft_date, business_start_date, business_end_date, business_period, business_reason, ";
  sql+="business_x, business_y, address) VALUES('"+params.user_id+"', '";
  sql+=params.draft+"', '"+params.start+"', '"+params.end+"', "+params.period+", '"+params.reason+"', "+ params.x+", "+ params.y+", '";
  sql+=params.address + "')";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
		console.log(err);
	});
});

app.get("/selectalert/:user_id", (req, res) => {
  var params = req.params;
  params.user_id = params.user_id.replace(":", "");
  sql = "SELECT * FROM ALERT WHERE user_id='"+params.user_id+"' ";
	connection.query(sql, (err, rows, fields) => {
		res.send(rows);
		console.log(rows);
	});
});
app.get("/deletealert/:user_id", (req, res) => {
  let { user_id } = req.params;
  user_id=user_id.replace(":", "");
  sql = "DELETE FROM ALERT WHERE user_id='" + user_id +"'";
  connection.query(sql, (err, rows, fields) => {
    console.log(sql);
    res.send(rows);
    console.log(rows);
  });
});
app.listen(PORT, () => console.log(`server on : http://localhost:${PORT}`))
