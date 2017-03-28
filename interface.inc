<?php
GLOBAL $theInterface; //------ This globalizes the interface variable.

//------ This section defines the interface when the user is logged in as a guest.
$theInterface[guest][Menu] = "guestMenu.html";
$theInterface[guest][SubMenu] = "guestSubMenu.html";
$theInterface[guest][Pages] = array (
	welcomePage => "welcomePage.html",
	contactUs => "contactUs.html",
	localDIs => "localDIs.html",
	loginForm => "loginForm.html",
	latestNews => "latestNews.html",
	registrationForm => "registrationForm.html",
	template => "template.html",
);

//------ This section defines the interface when the user is logged in as a driving instructor
$theInterface[di][Menu] = "userMenu.html";
$theInterface[di][SubMenu] = "diSubMenu.html";
$theInterface[di][Pages] = array (
	welcomePage => "diWelcomePage.html",
	latestNews => "latestNews.html",	
	clients => "diClients.html",
	availability => "diAvailability.html",
	appointments => "diAppointments.html",
	trackProgress => "diTrackProgress.html",
	fee => "diFee.html",
	transactions => "diTransactions.html",
	communicate => "diCommunicate.html",
	settings => "userSettings.html",
	contactUs => "contactUs.html",	
	logout => "logout.html",		
	);

//------ This section defines the interface for when the user is logged in as a learner driver
$theInterface[learner][Menu] = "userMenu.html";
$theInterface[learner][SubMenu] = "learnerSubMenu.html";
$theInterface[learner][Pages] = array (
	welcomePage => "learnerWelcomePage.html",
	latestNews => "latestNews.html",	
	localDIs => "localDIs.html",
	appointments => "learnerAppointments.html",
	trackProgress => "learnerTrackProgress.html",
	credits => "learnerCredits.html",
	transactions => "learnerTransactions.html",
	communicate => "learnerCommunicate.html",
	settings => "userSettings.html",	
	contactUs => "contactUs.html",
	logout => "logout.html",	
	);

//------ This section defines the interface for when the user is logged in as an administrator
$theInterface[admin][Menu] = "adminMenu.html";
$theInterface[admin][SubMenu] = "adminSubMenu.html";
$theInterface[admin][Pages] = array (
	welcomePage => "adminWelcomePage.html",
	publisher => "publisher.html",	
	verifyDIs => "verifyDIs.html",
	userProfiles => "userProfiles.html",
	settings => "adminSettings.html",	
	logout => "logout.html",	
	);
?>