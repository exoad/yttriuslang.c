const { MessageEmbed } = require("discord.js");
const config = require("../../configs/config.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
const superagent = require("superagent");
const colors = require("../../configs/colors.json");
const ready = require("../../configs/ready.json");
module.exports = {
  config: {
    name: `set`,
    category: "",
    description: "",
    aliases: [``],
  },
  run: async (bot, message, args) => {
    if (message.author.id == config.owner) {
      try {
        let choice = args[0];
        if (args[0] == "status") {
          let sta = args[1];
          bot.user.setPresence({ activity: { name: sta }, status: "online" });
          message.channel.send(
            "```xml\n<!-- Entered Value accepted -->\n</br>Changed 1.0 Condition: 0</br>\n<to>" +
              sta +
              "</to>```"
          );
        } else {
          message.channel.send("compilation failed on error: unknown");
        }
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
    }
  },
};
