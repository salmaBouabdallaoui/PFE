document.addEventListener("DOMContentLoaded", function () {
    const alertButton = document.getElementById("alertButton");
    const alertDropdown = document.getElementById("alertDropdown");
    const alertList = document.getElementById("alertList");
    const alertCount = document.getElementById("alertCount");

    let alerts = ["🔥 Température élevée dans la serre"];

    // Toggle alert dropdown
    alertButton.addEventListener("click", function () {
        alertDropdown.style.display = (alertDropdown.style.display === "block") ? "none" : "block";
    });

    // Function to update alert count
    function updateAlertCount() {
        if (alerts.length > 0) {
            alertCount.textContent = alerts.length;
            alertCount.style.display = "inline-block";
        } else {
            alertCount.style.display = "none";
        }
    }

    // Add a new alert dynamically
    function addAlert(message) {
        alerts.push(message);
        let li = document.createElement("li");
        li.textContent = message;
        alertList.appendChild(li);
        updateAlertCount();
    }

    // Example: Simulate a new alert after 5 seconds
    setTimeout(() => {
        addAlert("💧 Faible humidité du sol");
    }, 5000);

    // Mark alerts as read when clicking inside the dropdown
    alertDropdown.addEventListener("click", function () {
        alerts = [];
        alertList.innerHTML = "";
        updateAlertCount();
    });

    updateAlertCount();
});