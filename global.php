<?php
session_start();	//------ Start the session. This helps identify the user who is viewing the page by storing the information on their computer in the form of cookie.

GLOBAL $validation;	//------ Globalize the variable

include_once ("database.inc");	//------ Include the database module
include_once ("interface.inc");	//------ Include the interface module
include_once ("template.inc");	//------ Include the template module
include_once ("functions.inc");	//------ Include the functions module

if (!isActiveProfile($_SESSION[profileID])&&$_SESSION[profileID]!="xxxxxx"){	//------ If the user is not suspended and the user is not an administrator, reset the profile ID to blank.
	$_SESSION[profileID]="";
}

$_SESSION[userRank] = getUserRank($_SESSION[profileID]);	//------ Get the rank of the user depending on the profile ID.

$validation[AlphaNumeric] = "^[A-Za-z0-9]{1,}$";//------ alphabets and numbers
$validation[Alphabets] = "^[A-Za-z]{1,}$";		//------ albhabets
$validation[Numbers] = "^[0-9]{1,}$";	//------ numbers
$validation[AlphaNumericWithSpaces] = "^[A-Za-z0-9 ]{1,}$";	//alphabets and numbers
$validation[Name] = "^[A-Za-z -]{1,}$";	//alphabets, spaces and hyphens
$validation[Telephone] = "^0[0-9]{10}$";//telephone number starting with 0 and 11 digits long
$validation[EmailAddress] = "^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$"; //standard email address format
$validation[Postcode] = "^([A-PR-UWYZ0-9][A-HK-Y0-9][AEHMNPRTVXY0-9]?[ABEHMNPRVWXY0-9]? {0,1}[0-9][ABD-HJLN-UW-Z]{2}|GIR 0AA)$"; //standard UK postcode format
?>