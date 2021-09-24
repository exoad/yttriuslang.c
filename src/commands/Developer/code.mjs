// @ts-nocheck
import "panlang";
const { clang, c } = Engines.c;
//const Database = require("easy-json-database");
const { MessageEmbed } = require("discord.js");
const config = require("../../../configs/token.json");
const resource = require("../../../configs/resource.json");
module.exports = {
  config: {
    name: `lp`,
    category: "",
    description: "",
    aliases: [``],
  },
  run: async (bot, message) => {
    try {
      //const db = new Database("db/registry_user.json");
      if (message.author.id != config.owner_id) return;
      else {
        message.channel.send("Ho");
      }
    } catch (err) {
      console.error(err);
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

