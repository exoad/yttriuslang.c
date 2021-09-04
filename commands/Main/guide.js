const { MessageEmbed } = require("discord.js");
const config = require("../../configs/token.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
const superagent = require("superagent");
const token = require("../../configs/token.json");
const colors = require("../../configs/colors.json");
module.exports = {
  config: {
    name: `guide`,
    category: "",
    description: "",
    aliases: [`howto`],
  },
  run: async (bot, message, args) => {
    try {
      const embed = new MessageEmbed()
        .setTitle("How to use this bot | Bot prefix: " + token.prefix)
        .setDescription("This simple message will walk you through how to use this bot efficiently!\n*For further support, join my support sever using "+token.prefix+"support*")
        .addField("Command Decision", "You first must decide on what kind of command you want to use. Currently this bot has: `science` `Fun` `General Purpose` `Moderation`")
        .addField("Help Menu", "The help menu (`"+token.prefix+"help`) is just a simple list of commands and will not give you detailed information, however if you use the `"+token.prefix+"cmd` command along with a command name following it, you will recieve further information regarding that command!")
        .addField("Command Formatting", "Certain commands like `element`, requires input parameter. If you don't provide one, the bot will tell you the basic information you need to use that command. If no input is detected, the processes will not continue.")
        .addField("Science Commands", "Science commands are of course related in the field of science. This means, in order to use the command correctly, you must know what the command is going to be about. Sometimes they require parameters. In order to see the necessary parameters, just type the initial command out, and the bot will walk you through!")
        .setFooter("WIP | Work in progress! | Have suggestions? Join the support server")

      message.channel.send({embed});

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
