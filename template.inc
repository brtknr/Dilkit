<?php
GLOBAL $bodyMode; //Retrieves the global variable bodyMode that stylizes the boxes containing the text.
$bodyMode = true; //Set the bodymode as true.

//---------- Outputs the page header
function showHeader () {
GLOBAL $theInterface;

?>
<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>DILKIT :: Driving Instructor's and Learner's Kit</title>
<link rel="stylesheet" type="text/css" href="template.css">
</head>

<body topmargin="0" leftmargin="0" rightmargin="0" bottommargin="0">

<table width="100%" height="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td height="110" background="images/headerrighttile.jpg">
		<img src="images/header.jpg" width="640" height="110" alt="DILKIT :: Driving Instructor's and Learner's Kit" onclick="document.location='?p=welcomePage'"></td>
	</tr>
	<tr>
		<td height="25" background="images/headerandpagemergerighttile.jpg" style="background-image: url('images/topnavrighttile.jpg')">
		<table border="0" width="100%" cellspacing="0" cellpadding="0" style="background-image: url('images/topnavtile.jpg'); background-repeat: repeat-y" height="100%">
			<tr>
				<td>
				<table border="0" width="100%" cellpadding="10" cellspacing="0">
					<tr>
						<td>
<?
//---------- Embeds the Menu
	include ($theInterface[$_SESSION[userRank]][Menu]);

//---------- Outputs the gap between the menu and the navigation panel
?></td><td align = "right"><a href="?<?=$_SERVER[QUERY_STRING]?(strpos($_SERVER[QUERY_STRING],"&m=printable")?$_SERVER[QUERY_STRING]:"$_SERVER[QUERY_STRING]&m=printable"):"m=printable"?>" target="_new">Printable</a></td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td height="25" background="images/headerandpagemergerighttile.jpg">
		<img src="images/headerandpagemerge.jpg" width="640" height="25" alt=""></td>
	</tr>
	<tr>
		<td background="images/sidenavtile.jpg" valign="top" style="background-image: url(''); background-repeat: repeat-y">
		<table width="100%" height="100%" border="0" cellpadding="0" cellspacing="0">
			<tr>
				<td background="images/sidenavtile.jpg" width="200" valign="top" style="background-image: url('images/sidenavtile.jpg'); background-repeat: repeat-y">
				<?

//---------- Embeds the navigation panel
	include ($theInterface[$_SESSION[userRank]][SubMenu]);

//---------- Outputs the gap between the navigation panel and the content area.
?>
				</td>
				<td background="images/pagerighttile.jpg" width="100%">
				<table border="0" width="100%" cellspacing="5" cellpadding="0" style="background-image: url('images/pagetile.jpg'); background-repeat: repeat-y" height="100%">
					<tr>
						<td valign="top" width="100%" height="100%">
						<?
}


//---------- Outputs the Body Content Header
function bodyHeader ($header, $headerSize){
	GLOBAL $bodyMode;
	$borderColor = ($bodyMode)?"99CFB0":"6CB89C";
	$bodyMode= !$bodyMode;
?>								<table border="0" width="100%" cellspacing="5" cellpadding="0" style="border-style:solid; border-width:5px; padding:0;" bordercolor="#<?=$borderColor?>">
									<tr>
										<td>
										<h<?=$headerSize?>><?=$header?></h<?=$headerSize?>>
										</td>
									</tr>
									<tr>
										<td>										
<?
}

//---------- Embeds the Content of the body.
function showBody ($req) {
	GLOBAL $theInterface;

	include ($theInterface[$_SESSION[userRank]][Pages][$req]);
}

//---------- Outputs the Body Content Footer
function bodyFooter (){
?>
										</td>
									</tr>
								</table>
<?
}

//---------- Outputs the page footer.
function showFooter (){
?>						</td>
					</tr>
				</table>
				</td>
			</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td background="images/footerrighttile.jpg" height="30">
		<img src="images/footer.jpg" width="640" height="30" alt="© 2008 by Bharat Kunwar"></td>
	</tr>
</table>

</body>

</html>
<?
}
?>