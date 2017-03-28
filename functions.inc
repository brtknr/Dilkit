<?php
//------ General functions
function getNextID ($tableName){	//------ This function retrieves the next available ID from the  table specified in the parameter.
	$db = new database;	//------ Establish connection with the database.		//------ Connects to the database
	
	$result = $db->query("SELECT `".$tableName."_id` FROM `$tableName` ORDER BY `".$tableName."_id` DESC LIMIT 0, 1");	//------ Query to retrieve the highest ID value in the database table.
	
	if (!$result) $theID = false;	//------ If no result was found, it returns false.
	else $theID = $result[0][$tableName."_id"] + 1;	//------ If a result was returned, increment the value by one.

	return $theID;	//------ Return the value.
}



//------ Location Related
function getPostcodePrefix ($postcode){	//------ This function hacks a postcode so that only the first 2 letters and the following numerical value is retrned.
	$offset = 4; //------ Offset is 4 by default
	if (($postcodeStrlen = strlen ($postcode)) > 5 ) { // If the length of the postcode is greater than 5, offset is the value 3 less than the length of the postcode.
		$offset = $postcodeStrlen - 3; 
	} 
	return trim (substr ($postcode, 0, $offset)); //------ Return the characters from position 0 to position $offset
}

function getDistanceList ($subjectPostcode){	//------ This compiles a table of all the active driving instructors tagged with a value indicating how far away they live.
	$DIPostcodeList = "";	//------ Reset the list to blank

	$db = new database;	//------ Establish connection with the database.	//------ Establish connection to the database

	
	$subjectPostcode = getPostcodePrefix ($subjectPostcode);	//------ Get the postcode prefix of the subject, whether they are a guest or an active learner driver.
	
	$q = "SELECT `postcode_grid_north`, `postcode_grid_east` FROM `postcodes` WHERE `postcode` = '$subjectPostcode' LIMIT 1";	//------ Query to get the co-ordinates of the postcode from the database table.
	
	if ($result = $db->query($q)) {	//------ If the query returns any result, proceed with the enclosed action.
		//------ Assign the values from the first row to the local variables.
		$subjectGridNorth = $result[0][postcode_grid_north];
		$subjectGridEast  = $result[0][postcode_grid_east];
		
		if ($approvedDIIDs = getApprovedDIIDs()){	//------ Get the list of all the Driving Instructors whose status is marked as approved in the database table.
			foreach ($approvedDIIDs as $di){	//------ Retrieve one Driving Instructor at a time.
				//------ Retrieve the postcode and get the prefix of the postcode.
				$DIPostcode = getProfilePostcodeFromID($di[di_id]);
				$DIPostcodePrefix = getPostcodePrefix($DIPostcode);
				
				if (isActiveProfile($di[di_id])){	//------ Proceed only if the profile is active.
					
					$q = "SELECT `postcode_grid_north`, `postcode_grid_east` FROM `postcodes` WHERE `postcode` = '$DIPostcodePrefix'";
										
					$result = $db->query($q);	//------ Query the database to retrieve the co-ordinates associated with the postcode prefix.
			
					if ($result){	//------  Proceed if the co-ordinates are found.
						$x = $subjectGridNorth - $result[0][postcode_grid_north];	//------ Get the distance x by working out the difference between the 2 vertical coordinates
						$y = $subjectGridEast - $result[0][postcode_grid_east];	//------Get the distance y by working out the difference between the 2 horizontal coordinates
		
						$Distance = round(hypot($x, $y)/1000,1);	//------ Calculate the hypotenuse of the right angled triangle formed to calculate the distance.
						
						$DIName = getProfileNameFromID($di[di_id]);	//------ Retrieve the name of the Driving Instructor
						$DITelephone = getProfileTelephoneFromID($di[di_id]);	//------ Retrieve the telephone number of the Driving Instructor
						$DIEmail = getProfileEmailFromID($di[di_id]);	//------ Retrieve the Email address of the Driving Instructor

						$DIPostcodeList[] = array($di[di_id], $DIName, $DITelephone, $DIEmail, $DIPostcode, $Distance);	//------ Add the information about the instructor in the form of an array to the array variable.
					}
				}
			}
		}
	}
	return $DIPostcodeList;		//------ Return the list, return an empty list if nothing was found.
}




//------ Profile Related
function getUserRank ($profileID){	//------ This function retrieves the rank of the user depending on the profile ID specified.

	if (isDI($profileID)){	//------ If the profile ID is found in the DI table, return this value.
			return "di";
		} elseif (isLearner($profileID)){	//------ If the profile ID is found in the Learner Driver table, return this value.
			return "learner";
		} elseif ($profileID == "xxxxxx"){	//------ If the profile ID is specified as "xxxxxx", its a specially assigned profile ID to refer to an Administrator. Thus, this value is returned.
			return "admin";
		} else {	//------ In any other case, return the value guest.
			return "guest";
		} 
}

function getProfileIDFromEmail ($email){//------ This retrieves the Profile ID from the email address specified.
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `profile_id` FROM `profile` WHERE `profile_email`='$email';");	//------ Query the database table
	
	return $result[0][profile_id]; //------ Return the profile ID from the first row. If no profile ID is found, it automatically becomes interpreted as false as it returns a blank value.
}

function getProfileNameFromID ($theID){	//------ This retrieves the Name associated with the profile from the specified ID
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `profile_name` FROM `profile` WHERE `profile_id`='$theID';");	//------ Query the database table.
	
	return $result[0][profile_name];	//------ Returns the Name associated with the profile.
}

function getProfileTelephoneFromID ($theID){	//------ This retrieves the Telephone number associated with the profile from the specified ID
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `profile_telephone` FROM `profile` WHERE `profile_id`='$theID';");	//------ Query the database table.
	
	return $result[0][profile_telephone];	//------ Returns the telephone number associated with the profile.
}

function getProfileEmailFromID ($theID){	//------ This retrieves the Email address associated with the profile from the specified ID
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `profile_email` FROM `profile` WHERE `profile_id`='$theID';");	//------ Query the database table.

	return $result[0][profile_email];	//------ Returns the Email address associated with the profile.
}

function getProfileHouseNoFromID($profileID){	//------ This retrieves the House number associated with the profile from the specified ID
	$db = new database;	//------ Establish connection with the database.

	$result = $db->query("SELECT `profile_houseno` FROM `profile` WHERE `profile_id`='$profileID' AND `profile_active` = '1';");	//------ Query the database table.

	return $result[0]["profile_houseno"];	//------ Returns the House number associated with the profile.
}
function getProfilePostcodeFromID($profileID){	//------ This retrieves the Postcode associated with the profile from the specified ID
	$db = new database;	//------ Establish connection with the database.

	$result = $db->query("SELECT `profile_postcode` FROM `profile` WHERE `profile_id`='$profileID' AND `profile_active` = '1';");	//------ Query the database table.

	return $result[0]["profile_postcode"];	//------ Returns the Postcode associated with the profile.
}

function getPasswordMatch ($profileID, $password){	//------ This determines if the specified password matches the password in the record linked to the specified ID
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `profile_id` FROM `profile` WHERE `profile_id`='$profileID' and `profile_password`='$password';");	//------ Query the database table.
	
	return ($result)?true:false;	//------ Returns if the password matched with the profile ID or not.
}

function getProfiles(){	//------ This retrieves all the registered profile in the database.
	$db = new database;	//------ Establish connection with the database.

	return $db->query("SELECT `profile_id`, `profile_name`, `profile_houseno`, `profile_postcode`, `profile_telephone`, `profile_email`, `profile_password`, `profile_active`, `profile_updated` FROM `profile` ORDER BY `profile_active` DESC, `profile_updated` DESC;");	//------ Returns all the profiles in the database.
}

function isActiveProfile($profileID){	//------ This determines if the profile specified by the supplied ID is active or inactive
	$db = new database;	//------ Establish connection with the database.

	$result = $db->query("SELECT `profile_active` FROM `profile` WHERE `profile_id` = '$profileID';");	//------ Query the database table.

	return $result[0]["profile_active"]; //------ Returns if the profiles is active or has been suspended by the Administrator
}




//Administrator Related
function isAdmin ($username, $password){	//------ This determines if the supplied username and password matches what is in the file containing the Admin information.
	$handle = fopen ("login.info", "r");	//------ Opens the file containing the Admin information
	$theUsername = trim(fgets($handle, 20));	//------ Retrieves the username from the first line
	$thePassword = trim(fgets($handle, 20));	//------ Retrieves the password from the second line
	fclose ($handle);		//------ Closes the file

	if (strtolower($username) == strtolower($theUsername)){	//------ Checks if the usernames match
		if ($password==$thePassword){	//------ Checks if the passwords match
			return true;	//------ Returns true as the details match
		}else return false;	//------ Returns false because the passwords didnt match
	}else return false;	//------ Returns false because the usernames didnt match
}




//DI Related
function isDI($profileID){	//------ This determines if the specified profile ID is of a Driving Instructor
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT * FROM `di` WHERE `di_id`='$profileID';");	//------ Query the database table.

	return ($result)?true:false;	//------ Returns true or false depending on whether the record associated with the profile ID exists or not.
}

function getBuzzword($profileID){	//------ This retrieves the buzzword linked to the specified profile ID
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `di_buzzword` FROM `di` WHERE `di_id` = '$profileID';");	//------ Query the database table.

	return $result[0]["di_buzzword"];	//------ Returns the buzzword or if none is set, it returns blank.
}

function getApprovedDIIDs(){	//------ This retrieves the list of all the approved driving instructors
	$db = new database;	//------ Establish connection with the database.
	
	return $db->query("SELECT `di_id` FROM `di` WHERE `di_approved` = '1';");	//------ Returns the list of di ID of all approved driving instructors.
}

function isApprovedDI($profileID){	//------ This retrieves the approval status of the Driving Instructor
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `di_approved` FROM `di` WHERE `di_id` = '$profileID';");	//------ Query the database table.
	
	return $result[0][di_approved];	//------ If the DI has been approved by the Administrator, it returns true else it returns blank.
}

function getPendingDIList(){	//------ This retrieves the list of Driving Instructors pending verification along with their profile information.
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `di_id` FROM `di` WHERE `di_approved` = '0';");	//------ Query the database table.
	
	$PendingDIList = "";	//------ Resets the content of this variable to blank

	if ($result){	//------ If there are any pending DIs, execute the enclosed codes.
		foreach ($result as $di){	//------ Retrieve the pending DI one at a time.
			$DIBuzzword = getBuzzword($di[di_id]);	//------ This retrieves the buzzword
			$DIName = getProfileNameFromID($di[di_id]);	//------ This retrieves the name
			$DITelephone = getProfileTelephoneFromID($di[di_id]);	//------ This retrieves the telephone number
			$DIEmail = getProfileEmailFromID($di[di_id]);	//------ This retrieves the email address
			$DIPostcode = getProfilePostcodeFromID($di[di_id]);	//------ This retrieves the postcode
			
			$PendingDIList[] = array($di[di_id], $DIName, $DIBuzzword, $DITelephone, $DIEmail, $DIPostcode);	//------ This appends all the DI information indicated above to the list of pending  driving instructors
		}
	}

	return $PendingDIList;	//------ This returns the list array if the list has been compiled or it returns blank if there are no pending DIs.
}

function getDIIDfromBuzzword($theBuzzword){	//------ This retrieves the profile ID of the Driving Instructor from the specified buzzword
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `di_id` FROM `di` WHERE `di_buzzword` = '$theBuzzword';");	//------ Query the database table.
	
	return $result[0][di_id];	//------ Returns the profile ID of the Driving Instructor if a match is found. Otherwise, it returns blank.
}

function getDIIDfromLearnerID($profileID){	//------ This retrieves the profile ID of the Driving Instructor who is linked to the specified profile ID of the Learner Driver.
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `di_id` FROM `learner` WHERE `learner_id` = '$profileID';");	//------ Query the database table.
	
	return $result[0][di_id];	//------ Returns the DI's profile ID if the Learner Driver is associated with a DI. Otherwise, it returns blank.
}

function getClientProfiles($diID){	//------ This retrieves the list of clients who the DI specified by the diID is associated with.
	$db = new database;	//------ Establish connection with the database.

	//------ Query the database table.
	$q = 	"SELECT `profile`.`profile_id` , `profile`.`profile_name` , `profile`.`profile_houseno` , `profile`.`profile_postcode` , `profile`.`profile_telephone` , `profile`.`profile_email` , `profile`.`profile_password` , `profile`.`profile_active` , `profile`.`profile_updated` , `learner`.`learner_id` ".
			"FROM `profile` , `learner` ".
			"WHERE `learner`.`learner_id` = `profile`.`profile_id` ".
			"AND `learner`.`di_id` = '$diID' ".
			"ORDER BY `profile`.`profile_active` DESC, `learner`.`learner_passed` ASC, `profile_updated` DESC;";
			
	return $db->query($q);	//------ Returns the list if the DI has any clients, else it returns blank.
}

function getCurrentClients($diID){	//------ This retrieves the list of current clients who the DI specified by the diID is linked to.
	$db = new database;	//------ Establish connection with the database.
	
	//------ Query the database table.
	$q = 	"SELECT `profile`.`profile_id` , `profile`.`profile_name` , `profile`.`profile_houseno` , `profile`.`profile_postcode` , `profile`.`profile_telephone` , `profile`.`profile_email` , `profile`.`profile_password` , `profile`.`profile_active` , `profile`.`profile_updated` , `learner`.`learner_id` ".
			"FROM `profile` , `learner` ".
			"WHERE `learner`.`learner_id` = `profile`.`profile_id` ".
			"AND `learner`.`di_id` = '$diID' ".
			"AND `learner`.`learner_passed` = '0' ".
			"ORDER BY `profile`.`profile_active` DESC;";
			
	return $db->query($q);	//------ Returns the list if the DI has any current clients, else it returns blank.
}




//Learner Related

function isLearner($profileID){	//------ This determines whether the supplied profile ID is that of a learner.
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT * FROM `learner` WHERE `learner_id`='$profileID';");	//------ Query the database table.

	return ($result)?true:false;	//------ Returns true if the specified profile is that of a learner and false if not.
}

function hasPassed($profileID){	//------ This determines whether the client has passed or not.
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `learner_passed` FROM `learner` WHERE `learner_id` = '$profileID';");	//------ Query the database table.
	
	return $result[0][learner_passed];	//------ Returns true if the clients has passed and false if they haven't.
}



//News Related
function getActiveNews(){	//------ This retrieves the list of all the news items that are still marked as active.
	$db = new database;	//------ Establish connection with the database.
	
	return $db->query("SELECT `news_id`,`news_title`,`news_content`,`news_posted` FROM `news` WHERE `news_active` = '1' ORDER BY `news_posted` DESC;");	//------ Returns the list of news if there are any active news and blank variable if there arent any.
}

function getArchivedNews(){	//------ This retrieves the list of all the news items that have been marked as inactive.
	$db = new database;	//------ Establish connection with the database.

	return $db->query("SELECT `news_id`,`news_title`,`news_content`,`news_posted` FROM `news` WHERE `news_active` = '0' ORDER BY `news_posted` DESC;");	//------ Returns the list of news marked as inactive if any are found and a blank variable if there arent any.
}



//Communication Related
function getConversation($learnerID, $diID){	//------ Gets the log of conversation that took place between the specified profile IDs of learner drivers and driving instructors.
	$db = new database;	//------ Establish connection with the database.
	
	return $db->query("SELECT `message_id`,`message_content`,`message_generated`,`message_sender` FROM `message` WHERE `learner_id` = '$learnerID' AND `di_id` = '$diID' ORDER BY `message_generated` DESC;");	//------ Returns the log if any is found and returns blank if none is found.
}



//Transaction Related
function getRemainingLearnerCredit($profileID){	//------ This works out the remaining credits that the learner, specified by the profile ID still has available to spend on lessons.
	$diID = getDIIDFromLearnerID($profileID);	//------ This retrieves the profile ID of the DI who is associated with the learner.
	
	$totalCreditsPurchased = getTotalCreditsPurchased($profileID, $diID);	//------ This retrieves the total number of credits that have ever been purchased.
	$totalAppointments = getTotalAppointmentsBetween ($profileID, $diID);	//------ This retrieves the list of appointments that have already taken place.

	return $totalCreditsPurchased - $totalAppointments;	//------ Returns the difference between the two variables assigned above.
}

function getTotalCreditsPurchased($learnerID, $diID){	//------ This works out the total number of credits that have ever been purchased by a learner from a DI
	$theAmount = 0;	//------ The amound is reset to 0 initially.
	
	if ($transactions = getTransactionsBetween($learnerID,$diID)){	//------ Retrieve the list of transactions between the DI and the learner driver specified by the variables on the parameter.
		foreach ($transactions as $theTransaction){	//------ Every row of transaction is processed one at a time.
			$theAmount+= $theTransaction[transaction_amount];	//------ The amount of credits purchased is added to the Amount variable.
		}
	}
	return $theAmount;	//------ This returns the total number of credits that have ever been purchased by the learner driver from the DI
}

function getTransactionsBetween($learnerID, $diID){	//------ This compiles a list of all the transactions that have ever taken place between the specified learner driver and the driving instructor.
	$db = new database;	//------ Establish connection with the database.
	
	$theTransactions = "";	//------ Resets the content of the list of transactions to 0.
	
	$learnerTransactions =  $db->query("SELECT `transaction_id`,`transaction_amount`,`transaction_date_time`,`fee_id` FROM `transaction` WHERE `learner_id` = '$learnerID' ORDER BY `transaction_id` DESC;");	//------ Query the database table.
	
	if ($learnerTransactions){	//------ Proceed if there is a record of any transaction carried out by the specified learner driver.
		foreach ($learnerTransactions as $theTransaction){	//------ Retrieve the account of the transactions one row at a time.
			if ($theDIID = getDIIDFromFeeID($theTransaction[fee_id])){	//------ Retrieve the DI ID from the Fee ID contained in the account of the transaction.
				if ($theDIID == $diID){	//------ If the specified DI ID matched the retrieved DI ID, add the transactional account to the list of relevant transactions.
					$theTransactions[] = $theTransaction;
				}
			}
			
		}
	}
	return $theTransactions; // Return the list of relevant transactions or blank if no transactions  were found.
}

function offerLessonCapExists($offerLessonCap,$feeID){	//------ This is for checking whether the specified lesson cap already exists for the specified fee ID. This is useful for when offers are  being added to a fee where duplicate entries are not allowed.
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT * FROM `offer` WHERE `offer_lesson_cap` = '$offerLessonCap' AND `fee_id` = '$feeID';");	//------ Query the database table.
	
	return ($result)?true:false;	//------ Returns true or false depending on whether there were any results or not.
}

function discountPercentageIsWithinBounday($discountPercentage,$offerLessonCap,$feeID){	//------ This ensures that the specified discount Percentage for the specified Lesson Cap doesn't exceed the offer of a higher cap and is higher than the offer for a lower cap.
	$db = new database;	//------ Establish connection with the database.

	//------ Query the database table.
	$q = "SELECT * FROM `offer` WHERE ((`offer_lesson_cap` < '$offerLessonCap' AND `offer_discount_percentage` >= '$discountPercentage') ".
									"OR (`offer_lesson_cap` > '$offerLessonCap' AND `offer_discount_percentage` <= '$discountPercentage')) ".
									"AND `fee_id` = '$feeID' ORDER BY `offer_lesson_cap` DESC;";
	
	$result = $db->query($q);	//------ Query the database table.

	return ($result)?false:true;	//------ Returns true or false depending on whether it meets the condition or not.
}

function getApplicableDiscount ($feeID, $creditAmount){	//------ This retrieves the offer that is redeemable depending on how many credits has been specified and which fee ID it is referring to.
	$db = new database;	//------ Establish connection with the database.

	$result = $db->query("SELECT `offer_discount_percentage` FROM `offer` WHERE `fee_id` = '$feeID' AND `offer_lesson_cap` <= '$creditAmount' ORDER BY `offer_lesson_cap` DESC;");	//------ Query the database table.
	
	return ($result[0][offer_discount_percentage])?($result[0][offer_discount_percentage]):0;	//------ Returns the percentage redeemable if there is an applicable offer, otherwise returns 0.
}

function getOfferDetailsFromFeeID ($feeID){	//------ This retrieves a detailed list of the offers that are associated with the specified feeID.
	$db = new database;	//------ Establish connection with the database.
	
	return $db->query("SELECT `offer_lesson_cap`,`offer_discount_percentage` FROM `offer` WHERE `fee_id` = '$feeID' ORDER BY `offer_lesson_cap` DESC;");	//------ Queries the database table and returns the list.
}

function getDIIDFromFeeID($feeID){	//------ Returns the DI's profile ID who the specified fee ID is associated to.
	$db = new database;	//------ Establish connection with the database.

	$result = $db->query("SELECT `di_id` FROM `fee` WHERE `fee_id` = '$feeID';");	//------ Query the database table.

	return $result[0][di_id];	//------ Returns the ID of the driving instructor.
}

function getFeeDetailsFromDIID($diID){	//------ This retrieves the detailed account of fees assigned by the specified di ID.
	$db = new database;	//------ Establish connection with the database.
	
	return $db->query("SELECT `fee_id`,`fee_amount`,`fee_active` FROM `fee` WHERE `di_id` = '$diID' ORDER BY `fee_active` DESC, `fee_id` DESC;");	//------ Query the database table and return the list of detailed account of fees associated wit h the specified di ID.
}

function getFeeAmount($feeID){	//------ This retrieves the amount of fee that is associated with the specified fee ID.
	$db = new database;	//------ Establish connection with the database.
	
	$result = $db->query("SELECT `fee_amount` FROM `fee` WHERE `fee_id` = '$feeID';");	//------ Query the database table.
	
	return $result[0]["fee_amount"];	//------ Returns the amount of fee.
}

function killFeesWithDIID($diID){	//------ This marks all the fees assigned by the specified di ID as inactive.
	$db = new database;	//------ Establish connection with the database.
	
	return $db->rawquery("UPDATE `dilkit`.`fee` SET `fee_active` = '0'  WHERE `di_id` = '$diID';");	//------ Query the database table and return true if successful.
}

function discount($theAmount,$thePercentage){	//------ This works out the exact amount of discount to 2 decimal places from the specified  amount and percentage.
return round(($theAmount*$thePercentage)/100,2);	//------ Returns the amount of discount redeemable.
}




//Appointments Related
function getTotalAppointmentsBetween($learnerID, $diID){	//------ This counts the number of appointments that have taken place between the specified learner driver and DI.
	$db = new database;	//------ Establish connection with the database.
	
	$q = "SELECT COUNT(*) AS `total_appointments` FROM `appointment` WHERE `learner_id` = '$learnerID' AND `di_id` = '$diID' AND `appointment_cancelled` = '0';";

	$result = $db->query($q);	//------ Query the database table.
	
	return $result[0][total_appointments];	//------ Returns the total number of appointments if any have been booked.
}

function isBooked($hour, $date, $diID){	//------ This determines if the specified slot for the specified DI is booked or not.
	$db = new database;	//------ Establish connection with the database.
	
	$q = "SELECT * FROM `appointment` WHERE `appointment_hour`='$hour' AND `appointment_date`='$date' AND `appointment_cancelled` = '0' AND `di_id`='$diID';";
	
	return $db->query($q);	//------ Returns true if the slot is booked and false if not.
}

function getUpcomingAppointments ($learnerID, $diID){	//------ This returns the list of all the appointments that are coming up between the specified learner driver and DI.
	$db = new database;	//------ Establish connection with the database.

	$q = "SELECT `appointment_id`,`appointment_date`,`appointment_hour`,`appointment_cancelled` FROM `appointment` WHERE `learner_id` = '$learnerID' AND `di_id` = '$diID' AND `appointment_date` >= DATE(NOW()) ORDER BY `appointment_date` ASC, `appointment_cancelled` ASC, `appointment_hour` ASC;";
	
	return $db->query($q);	//------ Query the database table.
}

function getPastAppointments ($learnerID, $diID){	//------ This returns the list of all the appointments that have taken place between the specified learner driver and DI.
	$db = new database;	//------ Establish connection with the database.

	$q = "SELECT `appointment_id`,`appointment_date`,`appointment_hour` FROM `appointment` WHERE `learner_id` = '$learnerID' AND `di_id` = '$diID' and `appointment_cancelled` = 0 AND `appointment_date` <= DATE(NOW()) ORDER BY `appointment_date` DESC, `appointment_hour` ASC;";
	
	return $db->query($q);	//------ Query the database table and return the list.
}


//Availability Related
function getAvailability($diID){	//------ This returns the list of all the slots where the DI has indicated as being available for appointments.
	$db = new database;	//------ Establish connection with the database.
	
	$availability = "";	//------ This resets the content of the list to be returned to blank.
	
	$q = "SELECT `availability_day`, `availability_hour` FROM `availability` WHERE `di_id` = '$diID';";

	if ($result = $db->query($q)){	//------ Query the database table and proceed if the list returned by the query is not empty.
		foreach ($result as $record){	//------ Retrieve the availability from the list one at a time.
			$availability[$record[availability_day]][$record[availability_hour]] = 1;	//------ Gives the slots value of 1 where the DI is available.
		}
	}
	
	return $availability;	//------ Return the list if not blank.
}


//Progress Related 
function getProgressRatings(){	//------ This retrieves the list of progress ratings and their legend that are available in the database.
	$db = new database;	//------ Establish connection with the database.

	$q = "SELECT `progress_rating_id`,`progress_rating_name` FROM `progress_ratings`;";	//------ Query the database table.
	
	return $db->query($q);	//------ Returns the list.
}

function getProgressTypes(){	//------ This retrieves the list of types of progress that are available in the database.
	$db = new database;	//------ Establish connection with the database.

	$q = "SELECT `progress_type_id`,`progress_type_name` FROM `progress_types`;";	//------ Query the database table.
	
	return $db->query($q);	//------ Returns the list
}

function getProgressFromAppointment($appointmentID){	//------ This retrieves the progresses that have been recorded for the specified appointment ID.
	$db = new database;	//------ Establish connection with the database.

	//------ Query the database table.
	$q = 	"SELECT `progress_types`.`progress_type_name` AS `progress_type_name` , `progress_ratings`.`progress_rating_name` AS `progress_rating_name` , `progress`.`progress_remarks` AS `progress_remarks` ".
			"FROM `progress_types` , `progress_ratings` , `progress` ".
			"WHERE `progress`.`appointment_id` = '$appointmentID' ".
			"AND `progress`.`progress_rating_id` = `progress_ratings`.`progress_rating_id` ".
			"AND `progress`.`progress_type_id` = `progress_types`.`progress_type_id` ".
			"ORDER BY `progress_types`.`progress_type_id` DESC,`progress_ratings`.`progress_rating_id` DESC;";
	
	return $db->query($q);	//------ Query the database table and return the list.
}

function progressAlreadyExists($learner_id, $di_id, $progress_type_id, $progress_rating_id){	//------ This determines if the same progress has already been recorded for the learner driver. For this, both, type of progress and the rating entry, must have already been made for that particular learner driver.
	$db = new database;	//------ Establish connection with the database.
	 
	 //------ Query the database table.
	 $q =	"SELECT * ".
			"FROM `progress`, `appointment` ".
			"WHERE `appointment`.`learner_id` = '$learner_id' ".
			"AND `appointment`.`di_id` = '$di_id' ".
			"AND `appointment`.`appointment_cancelled` = '0' ".
			"AND `progress`.`appointment_id` = `appointment`.`appointment_id` ".
			"AND `progress_type_id` = '$progress_type_id' ".
			"AND `progress_rating_id` = '$progress_rating_id';";
			
	return $db->query($q); //Queries and returns the list if the progress specified already exists, otherwise returns a blank list.
}
?>