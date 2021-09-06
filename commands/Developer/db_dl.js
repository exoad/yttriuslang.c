const Database = require("easy-json-database");
const { MessageEmbed, MessageAttachment } = require("discord.js");
const config = require("../../configs/token.json");
module.exports = {
  config: {
    name: `dl`,
    category: "",
    description: "",
    aliases: [``],
  },
  run: async (bot, message, args) => {
    try {
      let choice = args[0];
      const db = new Database("db/registry_user.json");
      if (message.author.id != config.owner_id) return;
      else {
        if (args[0] == "clear") {
        
          db.clear();
          message.channel.send("Cleared");
        } else if (args[0] == "all") {
          if(!db.all()) return;
          else {
            const dl = new MessageAttachment("db/registry_user.json");
            bot.channels.cache.get(config.side_channel).send(dl);
            message.channel.send("Informations sent to logs");
          }
        }
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
  },
};
