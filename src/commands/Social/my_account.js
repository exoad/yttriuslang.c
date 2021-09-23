// DEPRECATED : const Database = require("easy-json-database");
const { Database } = require('secure-db');
const { MessageEmbed } = require("discord.js");
const config = require("../../../configs/token.json");
const resource = require("../../../../configs/resource.json");
module.exports = {
  config: {
    name: `myaccount`,
    category: "",
    description: "",
    aliases: [`account`],
  },
  run: async (bot, message, args) => {
    try {
      message.channel.send("**Checking the database...**\n*Please be patient, this won't last long*").then((m) => {
        setTimeout(function () {
          const db = new Database("account");
          // DEPRECATED DEPENDENCY : const db = new Database("db/registry_user.json");
          if(!db.has(message.author.id)) {
            m.edit("**Failed to retrieve your account**\nReason: You are not registered");
          } else {
            m.edit("Your Account information below:");
            const embed = new MessageEmbed()
            .setTitle(message.author.username + "'s Public Account")
            .setDescription("See any anomalies? Use `" + config.prefix + "support` to report the issue!")
            .addField("")
          }

        }, 1500);
      })
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
