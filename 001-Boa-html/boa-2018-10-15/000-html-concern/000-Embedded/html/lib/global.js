/*
var messageNum;
var gongNum;
var musicNum;
var normalBackLight;
var standyNum;
var secondStandy;
var autoTestFlag;
var screenOrientation;
var layout;
var languageValue;
*/

function setUserName(pName)
{
    localStorage.setItem('userName', pName);
    console.log("pName = ", pName);
}

function setPassword(pPwd)
{
    localStorage.setItem('userPwd', pPwd);
    console.log("pPwd = ", pPwd);
}

function setMessageNum(pMessageNum)
{	
	localStorage.setItem('messageNum', pMessageNum);
	console.log("pMessageNum = ", pMessageNum);
}

function setGongNum(pGongNum)
{	
	localStorage.setItem('gongNum', pGongNum);
	console.log("pGongNum = ", pGongNum);
}

function setMusicNum(pMusicNum)
{	
	localStorage.setItem('musicNum', pMusicNum);
	console.log("pMusicNum = ", pMusicNum);
}

function setNormalBackLight(pNormalBackLight)
{	
	localStorage.setItem('normalBackLight', pNormalBackLight);
	console.log("pNormalBackLight = ", pNormalBackLight);
}

function setStandyNum(pStandyNum)
{	
	localStorage.setItem('standyNum', pStandyNum);
	console.log("pStandyNum = ", pStandyNum);
}

function setSecondStandy(pSecondStandy)
{	
	localStorage.setItem('secondStandy', pSecondStandy);
	console.log("secondStandy = ", pSecondStandy);
}

function setAutoTestFlag(pAutoTestFlag)
{	
	localStorage.setItem('autoTestFlag', pAutoTestFlag);
	console.log("pAutoTestFlag = ", pAutoTestFlag);
}

function setScreenOrientation(pScreenOrientation)
{	
	localStorage.setItem('screenOrientation', pScreenOrientation);
	console.log("pScreenOrientation = ", pScreenOrientation);
}

function setLayout(pLayout)
{	
	localStorage.setItem('layout', pLayout);
	console.log("pLayout = ", pLayout);
}

function setLanguageValue(pLanguageValue)
{	
	localStorage.setItem('languageValue', pLanguageValue);
	console.log("pLanguageValue = ", pLanguageValue);
}

/**/
function getUserName()
{
    var userName = localStorage.getItem('userName');
    return userName;
}

function getUserPwd()
{
    var userPwd = localStorage.getItem('userPwd');
    return userPwd;
}

function getMessageNum()
{
    var messageNum = localStorage.getItem('messageNum');
	return messageNum;
}

function getGongNum()
{
    var gongNum = localStorage.getItem('gongNum');
	return gongNum;
}

function getMusicNum()
{
    var musicNum = localStorage.getItem('musicNum');
	return musicNum;
}

function getNormalBackLight()
{
    var normalBackLight = localStorage.getItem('normalBackLight');
	return normalBackLight;
}

function getStandyNum()
{
    var standyNum = localStorage.getItem('standyNum');
	return standyNum;
}

function getSecondStandy()
{
    var secondStandy = localStorage.getItem('secondStandy');
	return secondStandy;
}

function getAutoTestFlag()
{
    var autoTestFlag = localStorage.getItem('autoTestFlag');
	return autoTestFlag;
}

function getScreenOrientation()
{
    screenOrientation = localStorage.getItem('screenOrientation');
	return screenOrientation;
}

function getLayout()
{
    var layout = localStorage.getItem('layout');
	return layout;
}

function getLanguageValue()
{ 
    var languageValue = localStorage.getItem('languageValue');
	return languageValue;
}
