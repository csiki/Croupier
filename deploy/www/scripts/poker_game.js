function clone(obj) {
    // Handle the 3 simple types, and null or undefined
    if (null == obj || "object" != typeof obj) return obj;

    // Handle Array
    if (obj instanceof Array) {
        var copy = [];
        for (var i = 0, len = obj.length; i < len; i++) {
            copy[i] = clone(obj[i]);
        }
        return copy;
    }

    if (obj instanceof createjs.DisplayObject) {
        return obj.clone(true);
    }

    // Handle Object
    if (obj instanceof Object) {
        var copy = {};
        for (var attr in obj) {
            if (obj.hasOwnProperty(attr)) copy[attr] = clone(obj[attr]);
        }
        return copy;
    }

    throw new Error("Unable to copy obj! Its type isn't supported.");
}

var eventsJSONLen = 0;
var suits = ["HEARTS", "DIAMONDS", "CLUBS", "SPADES"];
var ranks = ["DEUCE", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT",
                "NINE", "TEN", "JACK", "QUEEN", "KING", "ACE"];
var cardNames = [];

var playerPositions = [[260, 342], [79, 328], [44, 142], [146, 60], [490, 141], [440, 330]];
var cardPositions = [[225, 262], [110, 262], [52, 177], [140, 100], [407, 177], [340, 262]];
var dealerPosition = [265, 130];
var cardsOnTablePosition = [170, 177];

var cardScale = 0.3;

var stage;
var loadqueue;

var loadingCircle;
var loadingCircleGoesLeft = false;
var loadingText;
var loadingBackground;

var loadComleted = false;

var cardsSpriteSheet;
var tableImage;
var eventText;
var titleText;

var running = false;
var animationRunning = false;

var cardsOnTable = [];
var chipsOnTable = 0;
var chipsOnTableText;

var dealerButton;
var dealerButtonOwner = -1;

var bots = {};

var animations = [];
var speed = 1;
var desiredSpeed = 1;

var currentEvent = -1;
var targetEvent = -1;
var lastAddedObjectID = 0;

var playButton, speedSlider;
var selectedRow;

var states = {};

function Bot(botID) {
    this.botID = botID;
    this.cards = [];
    this.name = "Bot";
    this.chips = 0;
    this.nameText = null;
    this.chipsText = null;
    this.seat = 0;
    this.hasLeft = false;
    this.getChips = function () {
        return this.chips;
    }
    this.setChips = function (amount) {
        this.chips = amount;
        this.chipsText.text = amount;
    }
}

function addBasicObjects() {
    stage.addChild(tableImage);
    stage.addChild(dealerButton);
    stage.addChild(eventText);
    stage.addChild(chipsOnTableText);
    stage.addChild(titleText);
}

function setLoading(val) {
    if (val) {
        stage.addChild(loadingBackground);
        stage.addChild(loadingText);
        stage.addChild(loadingCircle);
    }
    else {
        stage.removeChild(loadingBackground);
        stage.removeChild(loadingText);
        stage.removeChild(loadingCircle);
    }
}

function init() {
    stage = new createjs.Stage("gameCanvas");

    createjs.Ticker.addEventListener("tick", tick);
    createjs.Ticker.setFPS(40);

    loadqueue = new createjs.LoadQueue(false);
    loadqueue.on("complete", downloadFinished, this);
    loadqueue.loadManifest([
        { id: "table", src: "images/game/table.png" },
        { id: "cards", src: "images/cards.png" },
        { id: "dealer", src: "images/game/dealer_button.png" }
    ]);

    loadingBackground = new createjs.Shape();
    loadingBackground.graphics.beginFill("#333333").rect(0, 0, stage.canvas.width, stage.canvas.height);

    loadingText = new createjs.Text("Please wait", "20px Arial", "#ffffff");
    loadingText.x = stage.canvas.width / 2 - loadingText.getBounds().width / 2;
    loadingText.y = stage.canvas.height / 2 - 50;
    loadingText.textBaseline = "top";

    loadingCircle = new createjs.Shape();
    loadingCircle.graphics.beginFill("blue").drawCircle(0, 0, 5);
    loadingCircle.x = 200;
    loadingCircle.y = stage.canvas.height / 2;
    var f = function () {
        createjs.Tween.get(loadingCircle)
                 .to({ x: 325 }, 700).to({ x: 200 }, 700).call(f);
    }
    f();

    titleText = new createjs.Text("Visual log player", "15px Arial", "#ffffff");
    titleText.x = stage.canvas.width - titleText.getBounds().width - 5;
    titleText.y = 5;
    titleText.textBaseline = "top";

    setLoading(true);
    for (var s = 0; s < suits.length; ++s) {
        for (r = 0; r < ranks.length; ++r) {
            cname = suits[s] + " " + ranks[r];
            cardNames.push(cname);
        }
    }
    cardNames.push('NULL NULL');

    //stage.on("stagemouseup", function (evt) {
    //    console.log("stageX/Y: " + evt.stageX + "," + evt.stageY); // always in bounds
    //});

    for (var k in eventsJSON) {
        eventsJSONLen++;
    }

    playButton = $("#playButton");
    speedSlider = $("#speedSlider");
    $(document).keydown(function (e) {
        if (e.which == 38) { //up
            gotoEvent(currentEvent - 1);
            e.preventDefault();
            return false;
        }
        else if (e.which == 40) { //down
            gotoEvent(currentEvent + 1);
            e.preventDefault();
            return false;
        }
        else if (e.which == 32) { //space
            tooglePlayGame();
            e.preventDefault();
            return false;
        }
        return true;
    });
}

function tick(event) {
    if (!loadComleted) {
        stage.update();
        return;
    }

    if (animations.length != 0 && !animationRunning) {
        animationFinished();
        return;
    }
    if (running && !animationRunning) {
        if (currentEvent != eventsJSONLen - 1) {
            currentEvent++;
            parseEvent(eventsJSON[currentEvent]);
            eventText.text = "#" + (currentEvent + 1);
            if (targetEvent == -1 || targetEvent == currentEvent) {
                focusRow();
            }
            if (animations.length == 0) { //if there is no animation, so save and stop now
                saveState();
                if (targetEvent == currentEvent) {
                    targetEvent = -1;
                    speed = desiredSpeed;
                    running = false;
                    playButton.attr("disabled", false);
                    speedSlider.slider("option", "disabled", false);
                }
            }

        }
    }
    if (targetEvent != -1)
        setLoading(true);
    stage.update();
    if (targetEvent != -1)
        setLoading(false);
}

function downloadFinished() {

    tableImage = new createjs.Bitmap(loadqueue.getResult("table"));
    tableImage.cache(0, 0, 410, 221);

    tableImage.x = stage.canvas.width / 2 - tableImage.getBounds().width / 2;
    tableImage.y = 100;

    dealerButton = new createjs.Bitmap(loadqueue.getResult("dealer"));
    dealerButton.cache(0, 0, 25, 25);
    dealerButton.x = dealerPosition[0];
    dealerButton.y = dealerPosition[1];
    dealerButton.visible = false;

    eventText = new createjs.Text("", "14px Arial", "#FFFFFF");
    eventText.x = 5;
    eventText.y = 5;
    eventText.textBaseline = "top";

    chipsOnTableText = new createjs.Text("", "16px Arial", "#FFFFFF");
    chipsOnTableText.x = dealerPosition[0] - 30;
    chipsOnTableText.y = dealerPosition[1];
    chipsOnTableText.textBaseline = "top";

    var data = {
        images: [loadqueue.getResult("cards")],
        frames: { width: 125, height: 181, count: 58, regX: 0, regY: 0 }
    };
    cardsSpriteSheet = new createjs.SpriteSheet(data);
    lastAddedObjectID = stage.children.length - 1;
    addBasicObjects();

    loadComleted = true;
    gotoEvent(0);
}

function setSpeed(val) {
    desiredSpeed = val;
    speed = desiredSpeed;
}

function focusRow() {
    if (selectedRow != null)
        selectedRow.children("td").removeClass("gameSelectedEvent");
    selectedRow = $("#logTable tbody tr").eq(currentEvent);
    selectedRow.parents("div").eq(0).scrollTop(selectedRow.position().top - selectedRow.parent().position().top - 120);
    selectedRow.children("td").addClass("gameSelectedEvent");
}

function gotoEvent(eventID) {
    if (eventID < 0 || eventID >= eventsJSONLen || targetEvent != -1)
        return;
    if (states[eventID] != undefined) {
        loadState(eventID);
        running = false;
    }
    else {
        playButton.attr("disabled", true);
        speedSlider.slider("option", "disabled", true);
        speed = 0;
        targetEvent = eventID;
        running = true;
    }
}

function saveState() {
    if (states[currentEvent] == undefined) {
        var s = {};
        s["stage"] = stage.clone(true);
        s["canvas"] = stage.canvas;

        s["bots"] = clone(bots);
        s["cardsOnTable"] = clone(cardsOnTable);
        s["chipsOnTable"] = clone(chipsOnTable);
        s["chipsOnTableText"] = clone(chipsOnTableText);
        s["dealerButton"] = clone(dealerButton);
        s["dealerButtonOwner"] = clone(dealerButtonOwner);
        s["eventText"] = clone(eventText);
        states[currentEvent] = s;
    }
}

function loadState(event) {
    currentEvent = event;
    var s = states[event];

    createjs.Tween.removeAllTweens();
    stage.autoClear = true;
    stage.removeAllChildren();
    stage.update();

    stage = s["stage"].clone(true);
    stage.canvas = s["canvas"];

    chipsOnTable = clone(s["chipsOnTable"]);
    chipsOnTableText = clone(s["chipsOnTableText"]);
    dealerButton = clone(s["dealerButton"]);
    dealerButtonOwner = clone(s["dealerButtonOwner"]);
    eventText = clone(s["eventText"]);
    animations = [];
    addBasicObjects();

    bots = clone(s["bots"]);
    for (var i in bots) {
        stage.addChild(bots[i].cards[0]);
        stage.addChild(bots[i].cards[1]);
        stage.addChild(bots[i].nameText);
        stage.addChild(bots[i].chipsText);
    }
    cardsOnTable = clone(s["cardsOnTable"]);
    for (var i in cardsOnTable) {
        stage.addChild(cardsOnTable[i]);
    }
    focusRow();
}

function animationFinished() {
    if (animations.length != 0) {
        animationRunning = true;
        animations.shift()();
    }
    else {
        if (animationRunning == true) {
            saveState();
            if (targetEvent == currentEvent) {
                targetEvent = -1;
                speed = desiredSpeed;
                running = false;
                playButton.attr("disabled", false);
                speedSlider.slider("option", "disabled", false);
            }
        }
        animationRunning = false;
    }
}

function tooglePlayGame() {
    if (targetEvent != -1)
        return;
    if (running) {
        playButton.val("Play");
    }
    else {
        playButton.val("Pause");
    }
    running = !running;
}

function pauseGame() {
    running = false;
    playButton.val("Play");
}

function parseEvent(event) {
    var logger = event[0];
    var params = event[1].split(",");
    var functionName;
    if (params.length == 1) {
        params = event[1].split(" ");
        functionName = params.splice(0, 1)[0];
    }
    else {
        //get function name
        var a = params[0].split(" ");
        functionName = a.splice(0, 1)[0];
        //cut function name from the first element in array
        params[0] = params[0].substring(functionName.length + 1);
        params = [params];
    }
    if (logger != "0")
        params.splice(0, 0, logger)
    if (functionName.indexOf("get") != 0 && functionName.indexOf("can") != 0)
        executeFunctionByName(functionName, window, params);
}

function executeFunctionByName(functionName, context, argsArray) {
    //var argsArray = [].slice.call(arguments).splice(2);
    var namespaces = functionName.split(".");
    var func = namespaces.pop();
    for (var i = 0; i < namespaces.length; i++) {
        context = context[namespaces[i]];
    }
    if (context[func] == undefined)
        console.log(functionName);
    return context[func].apply(this, argsArray);
}

function animateMessage(message) {
    var t = new createjs.Text(message, "18px Arial", "#FFFFFF");
    t.x = stage.canvas.width / 2 - t.getBounds().width / 2;
    t.y = 10;
    t.alpha = 0;
    t.textBaseline = "top";
    stage.addChild(t);
    var f = function animate() {
        createjs.Tween.get(t)
             .to({ y: 30, alpha: 1 }, 300 * speed).wait(1000 * speed).to({ alpha: 0, visible: false }, 200 * speed)
             .call(removeText)
             .call(animationFinished);
        function removeText() {
            stage.removeChild(t);
        }
    }
    animations.push(f);
}

function showMessageAtBot(bot, message) {
    var t = new createjs.Text(message, "20px Arial", "#FFFFFF");

    t.x = bot.cards[1].x - t.getBounds().width / 2 - 2;
    t.y = bot.cards[1].y + 20;
    t.textBaseline = "top";

    var b = new createjs.Shape();
    b.graphics.beginFill("#5286ff").drawRoundRect(0, 0, t.getBounds().width + 4, t.getBounds().height + 4, 5);
    b.x = t.x - 2;
    b.y = t.y - 2;
    b.alpha = 0.9;
    stage.addChild(b);
    stage.addChild(t);
    var f = function animate() {
        createjs.Tween.get(b)
                .wait(1000 * speed).to({ alpha: 0, visible: false }, 200 * speed);
        createjs.Tween.get(t)
             .wait(1000 * speed).to({ alpha: 0, visible: false }, 200 * speed)
             .call(removeText)
             .call(animationFinished);
        function removeText() {
            stage.removeChild(t);
            stage.removeChild(b);
        }
    }
    animations.push(f);
}

function getChipsOnTable() {
    return chipsOnTable;
}

function setChipsOnTable(amount) {
    amount = parseInt(amount);
    chipsOnTable = amount;
    chipsOnTableText.text = amount == 0 ? "" : "Pot: " + amount;
}

function letsPoker() {
    for (var key in botsJSON) {
        var bot = new Bot(parseInt(botsJSON[key][0]));
        bot.name = botsJSON[key][1];
        bot.seat = parseInt(key);
        bot.nameText = new createjs.Text(bot.name, "14px Arial", "#FFFFFF");
        bot.nameText.x = playerPositions[bot.seat][0] - bot.nameText.getBounds().width / 2;
        bot.nameText.y = playerPositions[bot.seat][1];
        bot.nameText.textBaseline = "top";
        stage.addChild(bot.nameText);

        bot.chipsText = new createjs.Text(bot.chips, "bold 18px Arial", "#CC0000");
        bot.chipsText.x = bot.nameText.x;
        bot.chipsText.y = bot.nameText.y + 16;
        bot.chipsText.textBaseline = "top";
        stage.addChild(bot.chipsText);

        bots[bot.botID] = bot;
    }
}

function startingChips(amount) {
    for (var i in bots) {
        bots[i].setChips(parseInt(amount));
    }
}

function roundStarted(roundNum) {
    animateMessage("Round #" + roundNum + " started");
}

function dealing() {
    animateMessage("Dealing");
    function giveCard(bot, right) {
        return function () {
            var card = createCardSprite(52, bot.seat, right);
            var oldX = card.x;
            var oldY = card.y;
            card.x = dealerPosition[0];
            card.y = dealerPosition[1];
            stage.addChild(card);
            createjs.Tween.get(card)
                 .to({ x: oldX, y: oldY }, 100 * speed)
                 .call(animationFinished);
            bot.cards[right ? 1 : 0] = card;
        }
    }
    for (var key in bots) {
        if (!bots[key].hasLeft)
            animations.push(giveCard(bots[key], false));
    }

    for (var key in bots) {
        if (!bots[key].hasLeft)
            animations.push(giveCard(bots[key], true));
    }
}

function receiveCard(logger, suite, rank) {
    var cardID = cardNames.indexOf(suite + " " + rank);
    var bot = bots[logger];
    var cardIndex = -1;
    if (bot.cards[0].currentFrame == 52) {
        cardIndex = 0;
    }
    else if (bot.cards[1].currentFrame == 52) {
        cardIndex = 1;
    }

    var card = createCardSprite(cardID, bot.seat, cardIndex != 0);
    stage.removeChild(bot.cards[cardIndex]);
    bot.cards[cardIndex] = card;
    stage.addChild(card);

    animations.push(function () {
        window.setTimeout(animationFinished, 600 * speed);
    });
}

function rmDealerButton(botID) {
    dealerButtonOwner = -1;
    animations.push(function () {
        createjs.Tween.get(dealerButton)
             .to({ x: dealerPosition[0], y: dealerPosition[1] }, 300 * speed)
             .call(function () { dealerButton.visible = false; })
             .call(animationFinished);
    });
}

function addDealerButton(botID) {
    dealerButtonOwner = botID;
    dealerButton.visible = true;
    animations.push(function () {
        createjs.Tween.get(dealerButton)
             .to({ x: playerPositions[bots[dealerButtonOwner].seat][0] - dealerButton.getBounds().width / 2, y: playerPositions[bots[dealerButtonOwner].seat][1] - 27 }, 300 * speed)
             .call(animationFinished);
    });
}

function flop(cards) {
    animateMessage("Flop");
    function makeFunc(cardID) {
        return function () {
            var card = new createjs.Sprite(cardsSpriteSheet);
            card.gotoAndStop(cardID);
            card.scaleX = card.scaleY = cardScale;
            card.x = cardsOnTablePosition[0] + cardsOnTable.length * (card.getTransformedBounds().width + 3);
            card.y = cardsOnTablePosition[1];
            var oldX = card.x;
            var oldY = card.y;
            card.x = dealerPosition[0];
            card.y = dealerPosition[1];
            stage.addChild(card);
            createjs.Tween.get(card)
                 .to({ x: oldX, y: oldY }, 100 * speed)
                 .call(animationFinished);
            cardsOnTable.push(card);
        }
    }
    for (var key in cards) {
        var a = cards[key].split(" ");
        var cardID = cardNames.indexOf(a[0] + " " + a[1]);
        animations.push(makeFunc(cardID));
    }
    animations.push(function () {
        window.setTimeout(animationFinished, 1000 * speed);
    });
}

function turn(suite, rank) {
    animateMessage("Turn");
    animations.push(function () {
        var card = new createjs.Sprite(cardsSpriteSheet);
        card.gotoAndStop(cardNames.indexOf(suite + " " + rank));
        card.scaleX = card.scaleY = cardScale;
        card.x = cardsOnTablePosition[0] + cardsOnTable.length * (card.getTransformedBounds().width + 3);
        card.y = cardsOnTablePosition[1];
        var oldX = card.x;
        var oldY = card.y;
        card.x = dealerPosition[0];
        card.y = dealerPosition[1];
        stage.addChild(card);
        createjs.Tween.get(card)
             .to({ x: oldX, y: oldY }, 100 * speed)
             .call(animationFinished);
        cardsOnTable.push(card);
    });
    animations.push(function () {
        window.setTimeout(animationFinished, 1000 * speed);
    });
}

function river(suite, rank) {
    animateMessage("River");
    animations.push(function () {
        var card = new createjs.Sprite(cardsSpriteSheet);
        card.gotoAndStop(cardNames.indexOf(suite + " " + rank));
        card.scaleX = card.scaleY = cardScale;
        card.x = cardsOnTablePosition[0] + cardsOnTable.length * (card.getTransformedBounds().width + 3);
        card.y = cardsOnTablePosition[1];
        var oldX = card.x;
        var oldY = card.y;
        card.x = dealerPosition[0];
        card.y = dealerPosition[1];
        stage.addChild(card);
        createjs.Tween.get(card)
             .to({ x: oldX, y: oldY }, 100 * speed)
             .call(animationFinished);
        cardsOnTable.push(card);
    });
    animations.push(function () {
        window.setTimeout(animationFinished, 1000 * speed);
    });
}

function fold(logger) {
    showMessageAtBot(bots[logger], "Fold");

    animations.push(function () {
        createjs.Tween.get(bots[logger].cards[0])
             .to({ x: dealerPosition[0], y: dealerPosition[1] }, 100 * speed);
        createjs.Tween.get(bots[logger].cards[1])
             .to({ x: dealerPosition[0], y: dealerPosition[1] }, 100 * speed)
             .call(removeCards)
             .call(animationFinished);

        function removeCards() {
            stage.removeChild(bots[logger].cards[0]);
            stage.removeChild(bots[logger].cards[1]);
            bots[logger].cards = [];
        }
    });
}

function check(logger) {
    showMessageAtBot(bots[logger], "Check");
}

function call(logger, amount) {
    amount = parseInt(amount);
    showMessageAtBot(bots[logger], "Call " + amount);
    bots[logger].setChips(bots[logger].getChips() - amount);
    setChipsOnTable(getChipsOnTable() + amount);
}

function raise(logger, amount) {
    amount = parseInt(amount);
    showMessageAtBot(bots[logger], "Raise " + amount);
    bots[logger].setChips(bots[logger].getChips() - amount);
    setChipsOnTable(getChipsOnTable() + amount);
}

function allin(logger, amount) {
    amount = parseInt(amount);
    showMessageAtBot(bots[logger], "All in " + amount);
    bots[logger].setChips(bots[logger].getChips() - amount);
    setChipsOnTable(getChipsOnTable() + amount);
}

function revealCards(logger, array) {
    var bot = bots[logger];

    for (var i = 0; i < 2; i++) {
        var a = array[i].split(" ");
        var cardID = cardNames.indexOf(a[0] + " " + a[1]);
        var card = createCardSprite(cardID, bot.seat, i != 0);
        stage.removeChild(bot.cards[i]);
        bot.cards[i] = card;
        stage.addChild(card);
    }
    showMessageAtBot(bot, "Reveal cards");
    animations.push(function () {
        window.setTimeout(animationFinished, 300 * speed);
    });
}

function showdown() {
    animateMessage("Showdown");
}

function roundWinners(arr) {
    arr.splice(0, 1);
    var botnames = [];
    for (var i in arr) {
        botnames.push(bots[arr[i]].name);
    }
    if (arr.length == 1)
        animateMessage("Round winner: " + botnames[0]);
    else
        animateMessage("Round winners: " + botnames.join(", "));
}

function handOutPot(arr) {
    animateMessage("Hand out pot");
    for (var i in bots) {
        var bot = bots[i];
        if (bot.name == arr[1]) {
            bot.setChips(bot.getChips() + getChipsOnTable());
            setChipsOnTable(0);
            break;
        }
    }
}

function gameWinner(botID) {
    animateMessage("Game winner: " + bots[botID].name);
}

function collectCards(arr) {
    function removeCard(card) {
        return function () {
            createjs.Tween.get(card)
                 .to({ x: dealerPosition[0], y: dealerPosition[1] }, 100 * speed)
                 .call(removeCardFromStage)
                 .call(animationFinished);
            function removeCardFromStage() {
                stage.removeChild(card);
            }
        }
    }

    for (var key in bots)
        for (var k in bots[key].cards)
            animations.push(removeCard(bots[key].cards[k]));

    animations.push(function () {
        for (var key in bots) {
            bots[key].cards = [];
        }
        animationFinished();
    });

    for (var key in cardsOnTable) {
        animations.push(removeCard(cardsOnTable[key]));
    }

    animations.push(function () {
        cardsOnTable = [];
        animationFinished();
    });
}

function roundEnded() {
    animateMessage("Round ended");
    animations.push(function () {
        window.setTimeout(animationFinished, 1000 * speed);
    });
}

function createCardSprite(id, seat, isRight) {
    var card = new createjs.Sprite(cardsSpriteSheet);
    card.gotoAndStop(id);
    card.scaleX = card.scaleY = cardScale;
    if (!isRight)
        card.x = cardPositions[seat][0];
    else
        card.x = cardPositions[seat][0] + card.getTransformedBounds().width + 3;
    card.y = cardPositions[seat][1];
    return card;
}

function leave(botID) {
    var bot = bots[botID];
    animateMessage(bot.name + " left");
    bot.nameText.alpha = 0.4;
    bot.chipsText.alpha = 0.4;
    bot.hasLeft = true;
}

function rebuyOrLeave(botID) {
}

function preflop() {
}

function betRound() {
}

function burn(suite, rank) {
}