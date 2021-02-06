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
    ocument.getElementById("myIntro").classList.add("w3-show-inline-block");
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
function checkbox() { 
    var url_switch1 = "/switch1";
    $.ajax({
        url: url_switch1,
        dataType: "text",
        error: function() {
            alert("Error! Check your connection and try again.");
         },
        success: function (response) {
            if (response = '0') {
                alert("state 0");
            } else if (response = '1') { 
                alert("state 1");
                }
            }
        }
    )
}