//jshint esversion:6
const express = require("express");
const bodyParser = require("body-parser");
const ejs = require("ejs");
const mongoose = require("mongoose");

const app = express();
const import_from_excel = require('convert-excel-to-json');




app.use(express.static("public"));
app.set('view engine', 'ejs');
app.use(bodyParser.urlencoded({
  extended: true
}));


mongoose.connect("mongodb://localhost:27017/iaDB", {useNewUrlParser: true});

let usern ="";

const userSchema = {
  name: String,
  username: String,
  password: String,
  gender: String,
  age: Number,
  number: String,
  address: String,
  condition: String,
  dates: Array,
  exercise: Array,
  exercisedesc: Array
};
const User = mongoose.model("User", userSchema);

const adminSchema = {

  username: String,
  password: String,
  dates: Array,
  users: Array,
  reqdates: Array,
  requsers: Array
};
const Admin = mongoose.model("Admin", adminSchema);

const exerciseSchema = {
  exercise: String,
  exercisedesc: String
};
const Exercise = mongoose.model("Exercise", exerciseSchema);


app.get("/", function(req, res){
  res.render("login");
});

app.post("/login", function(req, res){
  usern = req.body.username;
  if(req.body.type == "User")
  {
  User.findOne({username:req.body.username},function(user){
  try{
    res.redirect("/Luser");
  }
  catch(err){
    console.log("No user found");
    res.redirect("/");
  }
  });
}
  else{
    Admin.find({username:req.body.username},function(user){
      try{
        res.redirect("/Ladmin");
      }
      catch(err){
        console.log("Wrong credentials for the admin");
        res.redirect("/");
      }
    });
  }
});

app.get("/register", function(req, res){
  res.render("register");
});

app.post("/register", function(req, res){
  usern = req.body.username;
  const newUser = new User({
      age:req.body.age,
      name: req.body.name,
      address:req.body.address,
      number: req.body.number,
      password: req.body.password,
      username: req.body.username,
      condition: req.body.condition,
      dates: [],
      exercise: [],
      exercisedesc: []
    });
    newUser.save();
    res.redirect("/")


});

app.get("/Luser", function(req, res){
  User.findOne({username:usern},function(err, users){
    res.render("Luser",{person:users});
  });

});

app.get("/Ladmin", function(req, res){
  Admin.findOne({username:usern}, function(err, doc){
    if(err){
      console.log(err);
    } else{
      res.render("Ladmin",{person:doc});
    }
  });

});

app.get("/makeappointment", function(req, res){
  User.find(function(err, doc){
    if(err){
      console.log(err);
    } else{
      Admin.find(function(err, admin){

          res.render("makeappointment",{person:doc, admin:admin});

      });

    }
  });
});

app.post("/makeappointment", function(req, res){
  Admin.findOne(function(err, admin){
  appointments_dates = admin.dates;
  appointments_dates.push(req.body.date_of_appointment);
  appointments_user = admin.users;
  appointments_user.push(req.body.user);
  Admin.updateOne({}, {dates: appointments_dates, users: appointments_user}, function(err){
        if(err){
        console.log(err);
      }else{
        res.redirect("/Ladmin");
      }
  });
  });

  User.findOne({name:req.body.user},function(err, user){
    appointment_date =user.dates;
    appointment_date.push(req.body.date_of_appointment);

  User.updateOne({name:req.body.user},{dates: appointment_date},function(err, user){});
  });

  const {google} = require('googleapis');
  require('dotenv').config();

  const CREDENTIALS = JSON.parse(process.env.CREDENTIALS);
  const calendarId = process.env.CALENDAR_ID;
  const SCOPES = 'https://www.googleapis.com/auth/calendar';
  const calendar = google.calendar({version : "v3"});
  const auth = new google.auth.JWT(
      CREDENTIALS.client_email,
      null,
      CREDENTIALS.private_key,
      SCOPES
  );
  const insertEvent = async (event) => {
      try {
          let response = await calendar.events.insert({
              auth: auth,
              calendarId: calendarId,
              resource: event
          });
          if (response['status'] == 200 && response['statusText'] === 'OK') {
              return 1;
          } else {
              return 0;
          }
      } catch (error) {
          console.log(`Error at insertEvent --> ${error}`);
          return 0;
      }
  };
   let event = {
      'summary': req.body.user+`'s appointment`,
      'description': `Time: `+req.body.time,
      'start': {
          'date': req.body.date,
           'timeZone': 'Asia/Kolkata'
       },
       'end': {
           'date': req.body.date,
         'timeZone': 'Asia/Kolkata'
      }
   };
   insertEvent(event)
       .then((res) => {
           console.log(res);
       })
       .catch((err) => {
           console.log(err);
       });
});

app.get("/list", function(req, res){
  User.find(function(err, users){
    if(err){
      console.log(err);
    } else{
      //bubble sort
      for(var i =0; i<users.length; i++){
        for(var j =0; j<users.length-i-1;j++){
          if(users[j].name.localeCompare(users[j+1].name)>0){
            var temp =users[j];
            users[j]= users[j+1];
            users[j+1]= temp
          }
        }
      }
      res.render("list",{person:users});


    }
  });

});

app.post("/search", function(req, res){
  User.find(function(err, users){
    if(err){
      console.log(err);
    } else{
      //bubble sort
      for(var i =0; i<users.length; i++){
        for(var j =0; j<users.length-i-1;j++){
          if(users[j].name.localeCompare(users[j+1].name)>0){
            var temp =users[j];
            users[j]= users[j+1];
            users[j+1]= temp
          }
        }
      }
      var searchedname = req.body.search;
      //linear search
      var index =4;
      for(var i=0;i<users.length;i++)
      {
        if(users[i].name === searchedname)
        {
          index =i;
          break;
        }
      }
      res.render("receipt",{person:users[index]})
    }
  });

});

app.post("/exporttopdf", function(req, res){
  User.findOne({username:req.body.username},function(err, doc){
    if(err){
      console.log(err);
    } else{

  const PDFGenerator = require('pdfkit')
  const fs = require('fs')
  let Output = new PDFGenerator
  Output.pipe(fs.createWriteStream('Invoice.pdf'))
  Output.text("INVOICE", { bold: true,
      underline: true,
      align: 'center'
  })
  Output.text(" ")
  Output.text(doc.name, {align: 'right'})
  Output.text(doc.address, {align: 'right'})
  Output.text(doc.number, {align: 'right'})
  Output.text(" ")
  Output.text("Amount: "+doc.dates.length*1000+" rupees", {align: 'left'})
  Output.text(" ")
  Output.text("Appointments this month:", { bold: true,underline: true, align: 'left'})

  for(var i =0;i<doc.dates.length;i++){
    Output.text((i+1)+". "+doc.dates[i], {align: 'left'})
  }
  Output.text(" ")
  Output.text(" ")
  Output.text(" ")
  Output.text(" ")
  Output.text("Signature of physiotherapist", {align: 'left'})
  Output.end()
  res.redirect("/list")
}
  });
});

app.get("/reqappointments", function(req, res){
res.render("reqappointments");

});

app.post("/reqappointments", function(req, res){
  Admin.findOne(function(err, admin){
  req_appointments_dates = admin.reqdates;
  req_appointments_dates.push(req.body.date_of_appointment);
  req_appointments_user = admin.requsers;
  req_appointments_user.push(usern);
  Admin.updateOne({}, {reqdates: req_appointments_dates, requsers: req_appointments_user}, function(err){
        if(err){
        console.log(err);
      }else{
        res.redirect("/Luser");
      }
  });
  });
});

app.get("/seereq", function(req, res){
  Admin.findOne(function(err, admin){
    if(err){
      console.log(err);
    } else{
      res.render("seereq",{admin:admin});
    }
  });
});


app.post("/remove_appointment", function(req, res){

  Admin.findOne(function(err, admin){
    var m = parseInt(req.body.option) - 1;
    var req_appointments_dates =admin.reqdates;
    var req_appointments_user =admin.requsers;
    req_appointments_dates.splice(m,1);
    req_appointments_user.splice(m,1);
  Admin.updateOne({}, {reqdates: req_appointments_dates, requsers: req_appointments_user}, function(err){
        if(err){
        console.log(err);
      }else{
        res.redirect("/seereq");
      }
  });
  });

});

app.get("/export", function(req, res){
  const excelexport = require('./export.js');

  const columns = [
      "ID",
      "Name",
      "Number",
      "Address",
      "Condition"
  ]

  const name_of_wksheet = 'Users';
  const path_of_file = 'exported.xlsx';

  User.find(function(err, doc){
    if(err){
      console.log(err);
    } else{

      excelexport(doc, columns, name_of_wksheet, path_of_file);
      res.redirect("/Ladmin");
    }
  });
});


app.get("/seeexercise", function(req, res){
  User.findOne({username:usern},function(err, doc){
    if(err){
      console.log(err);
    } else{
      res.render("seeexercise",{person:doc});


    }
  });
});

  app.get("/editexercise", function(req, res){
    User.find(function(err, doc){
      if(err){
        console.log(err);
      } else{
        Exercise.find(function(err,exercise){
          res.render("editexercise",{users:doc, exercise:exercise});
        });



      }
    });

});

app.get("/import", function(req, res){

  const data = import_from_excel({
      sourceFile: 'importing_users.xlsx',
      sheets:[{

          name: 'users',

  		header:{
              rows: 1
          },


          columnToKey: {
          	A: "name",
  			    B: "username",
  			    C: "password",
            D: "number",
            E: "address",
            F: "condition"
          }
      }]
  });

    User.insertMany(data.users, (err, res) => {
  	if (err){
      console.log(err);
    }});
    res.redirect("/list");
  });

app.post("/addexercise", function(req, res){
  User.findOne({name:req.body.user},function(err, doc){
    if(err){
      console.log(err);
    } else{
      exercises = doc.exercise;
      exercises.push(req.body.exercise);
      Exercise.findOne({exercise:req.body.exercise},function(err, exercise){
        exercisesdesc = doc.exercisedesc;
        exercisesdesc.push(exercise.exercisedesc);
        User.updateOne({name:req.body.user},{exercise:exercises, exercisedesc:exercisesdesc},function(err){
          res.redirect("/editexercise");
        });
     });

    }
  });
});

app.listen(3000, function() {
  console.log("Server started on port 3000");
});
