<?php
include ("global.php"); // Include the globally shared module

$p = $_GET[p]; //------ Retrieve information on which page has been requested
$m = $_GET[m]; //------ Retrieve if the page has been requested in Printable mode

if (!$theInterface[$_SESSION[userRank]][Pages][$p]){ // If the requested page isn't a valid page, set the page as the default page for that particular user
	$p = "welcomePage";
}

if (!$m) showHeader(); // Only if not in Printable mode, show the header.
showBody($p); // Show the body of the page.
if (!$m) showFooter(); // Only if not in Printable mode, show the footer.
?>