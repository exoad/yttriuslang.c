const { readdirSync } = require(`fs`);
const moment = require("moment");

module.exports = (bot) => {
    const load = dirs => {
        const commands = readdirSync(`./commands/${dirs}/`).filter(d => d.endsWith(`.js`));
        for (let file of commands) {
            let pull = require(`../commands/${dirs}/${file}`);
            bot.commands.set(pull.config.name, pull);
            if (pull.config.aliases) pull.config.aliases.forEach(a => bot.aliases.set(a, pull.config.name));
            //add moment load time
            console.log(`Command | ${pull.config.name} has loaded`)
        }
    };
    ["Main", "Tools", "Astronomy", "Chemistry", "Others", "Developer", "sci"].forEach(x => load(x));
}
