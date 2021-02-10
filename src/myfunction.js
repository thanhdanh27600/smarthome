// Open and close the sidebar on medium and small screens
function w3_open() {
    document.getElementById("mySidebar").style.display = "block";
    document.getElementById("myOverlay").style.display = "block";
}
function w3_close() {
document.getElementById("mySidebar").style.display = "none";
document.getElementById("myOverlay").style.display = "none";
}

// Change style of top container on scroll
window.onscroll = function () { myFunction() };
function myFunction() {
if (document.body.scrollTop > 80 || document.documentElement.scrollTop > 80) {
    document.getElementById("myTop").classList.add("w3-card-4", "w3-animate-opacity");
    document.getElementById("myIntro").classList.add("w3-show-inline-block");
} else {
    document.getElementById("myIntro").classList.remove("w3-show-inline-block");
    document.getElementById("myTop").classList.remove("w3-card-4", "w3-animate-opacity");
    }
}

// Accordions
function myAccordion(id) {
    var x = document.getElementById(id);
    if (x.className.indexOf("w3-show") == -1) {
        x.className += " w3-show";
        x.previousElementSibling.className += " w3-theme";
    } else {
        x.className = x.className.replace("w3-show", "");
        x.previousElementSibling.className =
            x.previousElementSibling.className.replace(" w3-theme", "");
    }
}

function changeOutput(pinToToggle) {
            var someUrl = "/digital_output/toggle";
            $.ajax({
                url: someUrl,
                dataType: "text",
                success: function(response) {
                    if (response == '0') {
                        $("#myImage").attr("src", "https://github.com/thanhdanh27600/smarthome/blob/main/bulb/pic_bulboff.gif?raw=true");
                    } else if (response == '1') {
                        $("#myImage").attr("src", "https://github.com/thanhdanh27600/smarthome/blob/main/bulb/pic_bulbon.gif?raw=true");
                    }
                },
                timeout: 2000
            })
}
function relay(relay_num) {
    var url_relay = "/relay?toggle=" + relay_num;
    $.ajax({
        url: url_relay,
        dataType: "text",
        error: function() {
            alert("Error! Check your connection and try again.");
        },
        success: function (response) {
                alert("At relay " + relay_num +". Response: state " + response);
            }
        }
    )
}

function dht() {
    // for check dht
    url_dht = "/dht";
    $.ajax({
        url: url_dht,
        dataType: "text",
        error: function () {
            alert("Error! Check your connection and try again.");
        },
        success: function (response) {
            if (response == "failed") {
                alert("Cannot read from DHT22");
                return;
            }
            else { 
                var obj = JSON.parse(response);
                document.getElementById("real_temp").innerText = obj.hic;
                document.getElementById("temparature").innerText = " : " + obj.temp + " &#8451";
                document.getElementById("relative_humid").innerText = " : " + obj.hic +"%";
            }
        }
    })
}

function updateTextENG() {
    document.getElementById("option1").innerText = "General Control";
    document.getElementById("option2").innerText = "Humidity & Temparature";
    document.getElementById("option3").innerText = "Setting";
    document.getElementById("relay1").innerText = "Relay 1";
    document.getElementById("relay2").innerText = "Relay 2";
    document.getElementById("relay3").innerText = "Front light";
    document.getElementById("relay4").innerText = "Alarm";
    document.getElementById("camera").innerText = "Security Camera";
    document.getElementById("refresh_button").innerText = "Update";
    document.getElementById("change_language").innerText = "Change language";
    document.getElementById("motion_sensor").innerText = "Motion Sensor";          
    document.getElementById("alarm_status").innerText = "Alarm Status";
    document.getElementById("heading_option1").innerText = "General Control Panel";
    document.getElementById("heading_option2").innerText = "DHT 22 Control Panel";
    document.getElementById("heading_option3").innerText = "Setting";    
}

function updateTextVIE() {
    document.getElementById("option1").innerText = "Dieu khien chung";
    document.getElementById("option2").innerText = "Nhiet do & Do am";
    document.getElementById("option3").innerText = "Cai dat";
    document.getElementById("relay1").innerText = "Relay 1";
    document.getElementById("relay2").innerText = "Relay 2";
    document.getElementById("relay3").innerText = "Den cong truoc";
    document.getElementById("relay4").innerText = "Chuong";
    document.getElementById("camera").innerText = "Security Camera";
    document.getElementById("refresh_button").innerText = "Cap nhat";
    document.getElementById("change_language").innerText = "Thay doi ngon ngu";
    document.getElementById("motion_sensor").innerText = "Kich hoat chuyen dong";
    document.getElementById("alarm_status").innerText = "Kich hoat chuong bao";
    document.getElementById("heading_option1").innerText = "Dieu khien cong tac";
    document.getElementById("heading_option2").innerText = "DHT22 Quan ly cam bien";
    document.getElementById("heading_option3").innerText = "Cai dat";
            
}



function option1() {
            reset();
            document.getElementById("option1").classList.add('w3-teal');
            document.getElementById("general_control").classList.add('d-block');
            document.getElementById("dht_22").classList.add('d-none');
            document.getElementById("general_setting").classList.add('d-none');
            document.getElementById("myIntro").innerText = "General Control";
}
function option2() {
            reset();
            document.getElementById("option2").classList.add('w3-teal');
            document.getElementById("general_control").classList.add('d-none');
            document.getElementById("dht_22").classList.add('d-block');
            document.getElementById("general_setting").classList.add('d-none');
            document.getElementById("myIntro").innerText = "Humidity & Temparature";
}
function option3() {
            reset();
            document.getElementById("option3").classList.add('w3-teal');
            document.getElementById("general_control").classList.add('d-none');
            document.getElementById("dht_22").classList.add('d-none');
            document.getElementById("general_setting").classList.add('d-block');
            document.getElementById("myIntro").innerText = "Setting";
}
function reset() {
            document.getElementById("option1").classList.remove('w3-teal');
            document.getElementById("option2").classList.remove('w3-teal');
            document.getElementById("option3").classList.remove('w3-teal');
            document.getElementById("general_control").classList.remove('d-none', 'd-block');
            document.getElementById("dht_22").classList.remove('d-none', 'd-block');
            document.getElementById("general_setting").classList.remove('d-none', 'd-block');
            document.getElementById("myIntro").innerText = "";
}
function handleLanguage() {
    if(document.getElementById("change_language").getAttribute("value") == "vie"){
        document.getElementById("change_language").setAttribute("value","eng");
        updateTextENG();
    }else{
        document.getElementById("change_language").setAttribute("value","vie");
        updateTextVIE();
    }          
}