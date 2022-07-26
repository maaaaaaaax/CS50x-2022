import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Create new transactions table manually:
# CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, uid INTEGER NOT NULL, type TEXT NOT NULL, share_count NUMERIC NOT NULL, symbol TEXT NOT NULL, share_price NUMERIC NOT NULL, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP);


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Show, for the user currently logged in, which stocks the user owns, the numbers of shares owned,
    # the current price of each stock, and the total value of each holding (i.e., shares times price).

    # Count the current holdings, by substracting sold shares from purchased shares
    portfolio = {}

    # Pull the purchase history for this user to count total shares owned of each company
    rows = db.execute("select * from transactions where uid = ? and type = 'buy';", session["user_id"])
    for row in rows:
        if row['symbol'] in portfolio:
            portfolio[row['symbol']] += row['share_count']
        else:
            portfolio[row['symbol']] = row['share_count']

    # Substract shares sold from shares purchased to show current ownership
    rows = db.execute("select * from transactions where uid = ? and type = 'sell';", session["user_id"])
    for row in rows:
        portfolio[row['symbol']] -= row['share_count']

    # Get user account cash balance
    rows = db.execute("select cash from users where id = ?;", session["user_id"])
    account_cash_balance = rows[0]['cash']
    account_total_value = account_cash_balance

    # For each position, get current price of each stock
    positions = []
    for symbol in portfolio:
        current_price = lookup(symbol)['price']
        total_value = portfolio[symbol] * current_price
        account_total_value += total_value
        position = {
            'symbol': symbol,
            'shares_owned': portfolio[symbol],
            'current_price': current_price,
            'total_value': total_value
        }
        positions.append(position)

    # Also display the user’s current cash balance along with a grand total (i.e., stocks’ total value plus cash).
    return render_template("index.html", positions=positions, account_cash_balance=account_cash_balance, account_total_value=account_total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted and user's input is not blank
        if not request.form.get("symbol") or len(request.form.get("symbol")) < 1:
            return apology("must provide symbol", 400)

        # Ensure share count is a positive integer
        isInt = True
        try:
            share_count = int(request.form.get("shares"))
        except ValueError:
            isInt = False
        if not isInt:
            return apology("invalid number of shares", 400)

        if share_count < 1:
            return apology("invalid number of shares", 400)

        # Call IEX Cloud API (iexcloud.io) for last price
        price = lookup(request.form.get("symbol"))
        if price == None:
            return apology("must provide valid symbol", 400)
        else:
            # Check if the user has enough money to buy the shares
            price_per_share = price['price']
            print("price_per_share: " + str(price_per_share))
            total_cost = price_per_share * share_count
            print("Total cost: " + str(total_cost))
            rows = db.execute("select cash from users where id = ?", session["user_id"])
            purchasing_power = rows[0]["cash"]
            print("purchasing power: " + str(purchasing_power))

            # Render an apology, without completing a purchase, if the user cannot afford the number of shares at the current price.
            if purchasing_power < total_cost:
                return apology("transaction failed: purchasing power too low", 400)
            else:
                # Process purchase in DB
                db.execute("INSERT INTO transactions (uid, type, share_count, symbol, share_price) VALUES (?, ?, ?, ?, ?);", session["user_id"], "buy", share_count, price['symbol'], price_per_share)
                # Update account balance
                new_balance = purchasing_power - total_cost
                db.execute("UPDATE users SET cash = ? where id = ?;", new_balance, session["user_id"])
                return redirect("/history")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    trades = db.execute("select * from transactions where uid = ?", session["user_id"])
    rows = db.execute("select cash from users where id = ?", session["user_id"])
    purchasing_power = rows[0]["cash"]
    return render_template("history.html", trades=trades, purchasing_power=purchasing_power)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted and user's input is not blank
        if not request.form.get("symbol") or len(request.form.get("symbol")) < 1:
            return apology("must provide symbol", 400)

        price = lookup(request.form.get("symbol"))
        if price == None:
            return apology("must provide symbol", 400)
        else:
            return render_template("quote.html", price=price)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # return apology("TODO")

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted and user's input is not blank
        if not request.form.get("username") or len(request.form.get("username")) < 1:
            return apology("must provide username", 400)

        # Ensure username is not already in use
            # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?;", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 0:
            return apology("invalid username and/or password", 400)

        # Ensure password was submitted and user's input is not blank
        elif not request.form.get("password") or len(request.form.get("password")) < 1:
            return apology("must provide password", 400)

        # Ensure confirm password was submitted AND user's input is not blank AND password is same as confirm password
        elif not request.form.get("confirmation") or len(request.form.get("confirmation")) < 1 or request.form.get("password") != request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # INSERT the new user into users, storing a hash of the user’s password, not the password itself.
        hash = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", request.form.get("username"), hash)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Render an apology if the client provides an invalid symbol or share count before checking DB
        if not request.form.get("symbol") or not request.form.get("shares") or len(request.form.get("shares")) < 1:
            return apology("must select symbol and shares", 400)

        # Confirm that share_count is a positive integer
        share_count = int(request.form.get("shares"))
        if not share_count > 0:
            return apology("must select symbol and shares", 400)

        # Render an apology if the user does not own that many shares of the stock.

            # Count, for the user currently logged in, which stocks the user owns, the numbers of shares owned,
            # the current price of each stock, and the total value of each holding (i.e., shares times price).

            # Count the current holdings, by substracting sold shares from purchased shares
        portfolio = {}
        symbol = request.form.get("symbol")

        # Pull the purchase history for this user to count total shares owned of each company
        rows = db.execute("select * from transactions where uid = ? and type = 'buy' and symbol = ?;", session["user_id"], symbol)
        for row in rows:
            if row['symbol'] in portfolio:
                portfolio[row['symbol']] += row['share_count']
            else:
                portfolio[row['symbol']] = row['share_count']

            # Substract shares sold from shares purchased to show current ownership
        rows = db.execute("select * from transactions where uid = ? and type = 'sell' and symbol = ?;", session["user_id"], symbol)
        for row in rows:
            portfolio[row['symbol']] -= row['share_count']

        if share_count > portfolio[symbol]:
            return apology("You don't own enough shares.", 400)

        # Process the transaction
            # Determine the share price
        price_per_share = lookup(symbol)['price']
        total_sale_value = price_per_share * share_count
        # Update transactions to reflect sale
        db.execute("INSERT INTO transactions (uid, type, share_count, symbol, share_price) VALUES (?, ?, ?, ?, ?);", session["user_id"], "sell", share_count, symbol, price_per_share)
        # Update user account balance with new cash
        rows = db.execute("select cash from users where id = ?", session["user_id"])
        account_balance = rows[0]['cash']
        account_balance += total_sale_value
        db.execute("UPDATE users SET cash = ? where id = ?", account_balance, session["user_id"])
        return redirect("/history")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        # Count, for the user currently logged in, which stocks the user owns, the numbers of shares owned,
        # the current price of each stock, and the total value of each holding (i.e., shares times price).

        # Count the current holdings, by substracting sold shares from purchased shares
        portfolio = {}

        # Pull the purchase history for this user to count total shares owned of each company
        rows = db.execute("select * from transactions where uid = ? and type = 'buy';", session["user_id"])
        for row in rows:
            if row['symbol'] in portfolio:
                portfolio[row['symbol']] += row['share_count']
            else:
                portfolio[row['symbol']] = row['share_count']

        # Substract shares sold from shares purchased to show current ownership
        rows = db.execute("select * from transactions where uid = ? and type = 'sell';", session["user_id"])
        for row in rows:
            portfolio[row['symbol']] -= row['share_count']

        return render_template("sell.html", portfolio=portfolio)