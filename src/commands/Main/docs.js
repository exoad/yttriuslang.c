//set this as a cooldown enforced command;
const talkedRecently = new Set();
const { MessageEmbed, DiscordAPIError } = require("discord.js");
const Discord = require("discord.js");
const config = require("../../../configs/token.json");
const chnl = require("../../../configs/chnl.json");
const resource = require("../../../configs/resource.json");
const superagent = require("superagent");
const colors = require("../../../configs/colors.json");
//const Database = require("easy-json-database");
const token = require("../../../configs/token.json");
module.exports = {
  config: {
    name: `docs`,
    category: "",
    description: "",
    aliases: [`doc`],
  },
  run: async (bot, message, args) => {
    try {

    } catch (err) {
      const embed = new MessageEmbed()
        .setTitle("Whoops, looks like something went wrong!")
        .setThumbnail(resource.aw_snap)
        .setDescription(
          "Use `" +
            config.prefix +
            "help` for a list of avaliable commands or use `" +
            config.prefix +
            "support` to join the support server!"
        )
        .setFooter("Still facing issues? Join the support server!");
      message.channel.send({ embed }).then((m) => {
        m.delete({ timeout: 5000 });
      });
    }
  },
};
