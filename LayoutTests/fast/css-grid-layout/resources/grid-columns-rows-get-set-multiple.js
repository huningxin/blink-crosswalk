description('Test that setting and getting grid-definition-columns and grid-definition-rows works as expected');

debug("Test getting |grid-definition-columns| and |grid-definition-rows| set through CSS");
testGridDefinitionsValues(document.getElementById("gridWithFixedElement"), "7px 11px", "17px 2px");
testGridDefinitionsValues(document.getElementById("gridWithPercentElement"), "53% 99%", "27% 52%");
testGridDefinitionsValues(document.getElementById("gridWithAutoElement"), "auto auto", "auto auto");
testGridDefinitionsValues(document.getElementById("gridWithEMElement"), "100px 120px", "150px 170px");
testGridDefinitionsValues(document.getElementById("gridWithThreeItems"), "15px auto 100px", "120px 18px auto");
testGridDefinitionsValues(document.getElementById("gridWithPercentAndViewportPercent"), "50% 120px", "35% 168px");
testGridDefinitionsValues(document.getElementById("gridWithFitContentAndFitAvailable"), "none", "none");
testGridDefinitionsValues(document.getElementById("gridWithMinMaxContent"), "min-content max-content", "max-content min-content");
testGridDefinitionsValues(document.getElementById("gridWithMinMaxAndFixed"), "minmax(45px, 30%) 15px", "120px minmax(35%, 10px)");
testGridDefinitionsValues(document.getElementById("gridWithMinMaxAndMinMaxContent"), "minmax(min-content, 30%) 15px", "120px minmax(35%, max-content)");
testGridDefinitionsValues(document.getElementById("gridWithFractionFraction"), "1fr 2fr", "3fr 4fr");
testGridDefinitionsValues(document.getElementById("gridWithFractionMinMax"), "minmax(min-content, 45px) 2fr", "3fr minmax(14px, max-content)");

debug("");
debug("Test the initial value");
var element = document.createElement("div");
document.body.appendChild(element);
shouldBe("getComputedStyle(element, '').getPropertyValue('grid-definition-columns')", "'none'");
shouldBe("getComputedStyle(element, '').getPropertyValue('grid-definition-rows')", "'none'");

debug("");
debug("Test getting and setting grid-definition-rows and grid-definition-columns through JS");
testGridDefinitionsSetJSValues("18px 22px", "66px 70px");
testGridDefinitionsSetJSValues("55% 80%", "40% 63%");
testGridDefinitionsSetJSValues("auto auto", "auto auto");
testGridDefinitionsSetJSValues("auto 16em 22px", "56% 10em auto", "auto 160px 22px", "56% 100px auto");
testGridDefinitionsSetJSValues("16em minmax(16px, 20px)", "minmax(10%, 15%) auto", "160px minmax(16px, 20px)");
testGridDefinitionsSetJSValues("16em 2fr", "14fr auto", "160px 2fr");
testGridDefinitionsSetJSValues("50% 12vw", "5% 85vh", "50% 96px", "5% 510px");

debug("");
debug("Test getting wrong values set from CSS");
var gridWithNoneAndAuto = document.getElementById("gridWithNoneAndAuto");
shouldBe("getComputedStyle(gridWithNoneAndAuto, '').getPropertyValue('grid-definition-columns')", "'none'");
shouldBe("getComputedStyle(gridWithNoneAndAuto, '').getPropertyValue('grid-definition-rows')", "'none'");

var gridWithNoneAndFixed = document.getElementById("gridWithNoneAndFixed");
shouldBe("getComputedStyle(gridWithNoneAndFixed, '').getPropertyValue('grid-definition-columns')", "'none'");
shouldBe("getComputedStyle(gridWithNoneAndFixed, '').getPropertyValue('grid-definition-rows')", "'none'");

debug("");
debug("Test setting and getting wrong values from JS");
testGridDefinitionsSetBadJSValues("none auto", "none auto");
testGridDefinitionsSetBadJSValues("none 16em", "none 56%");
testGridDefinitionsSetBadJSValues("none none", "none none");
testGridDefinitionsSetBadJSValues("auto none", "auto none");
testGridDefinitionsSetBadJSValues("auto none 16em", "auto 18em none");
testGridDefinitionsSetBadJSValues("-webkit-fit-content -webkit-fit-content", "-webkit-fit-available -webkit-fit-available");
testGridDefinitionsSetBadJSValues("auto minmax(16px, auto)", "minmax(auto, 15%) 10vw");
// Negative values are not allowed.
testGridDefinitionsSetBadJSValues("-10px minmax(16px, 32px)", "minmax(10%, 15%) -10vw");
testGridDefinitionsSetBadJSValues("10px minmax(16px, -1vw)", "minmax(-1%, 15%) 10vw");

function testInherit()
{
    var parentElement = document.createElement("div");
    document.body.appendChild(parentElement);
    parentElement.style.gridDefinitionColumns = "50px 1fr 'last'";
    parentElement.style.gridDefinitionRows = "101% 'middle' 45px";
    testGridDefinitionsValues(parentElement, "50px 1fr last", "101% middle 45px");

    element = document.createElement("div");
    parentElement.appendChild(element);
    element.style.gridDefinitionColumns = "inherit";
    element.style.gridDefinitionRows = "inherit";
    testGridDefinitionsValues(element, "50px 1fr last", "101% middle 45px");

    document.body.removeChild(parentElement);
}
debug("");
debug("Test setting grid-definition-columns and grid-definition-rows to 'inherit' through JS");
testInherit();

function testInitial()
{
    element = document.createElement("div");
    document.body.appendChild(element);
    element.style.gridDefinitionColumns = "150% 'middle' 55px";
    element.style.gridDefinitionRows = "1fr 'line' 2fr 'line'";
    testGridDefinitionsValues(element, "150% middle 55px", "1fr line 2fr line");

    element.style.gridDefinitionColumns = "initial";
    element.style.gridDefinitionRows = "initial";
    shouldBe("getComputedStyle(element, '').getPropertyValue('grid-definition-columns')", "'none'");
    shouldBe("getComputedStyle(element, '').getPropertyValue('grid-definition-rows')", "'none'");

    document.body.removeChild(element);
}
debug("");
debug("Test setting grid-definition-columns and grid-definition-rows to 'initial' through JS");
testInitial();
