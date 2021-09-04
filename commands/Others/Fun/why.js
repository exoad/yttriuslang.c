const superagent = require("superagent");
module.exports = {
  config: {
    name: `why`,
    category: "",
    description: "",
    aliases: [``],
  },
  run: async (bot, message, args) => {
    try {
      const { body } = await superagent.get("https://nekos.life/api/v2/why");

      const embed = new MessageEmbed()
        .setTitle("Why? Why? Why?")
        .setDescription(body.why)
        .setColor("RANDOM");

      message.channel.send({ embed });
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
