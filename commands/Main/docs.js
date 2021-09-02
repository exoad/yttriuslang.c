const { MessageEmbed } = require("discord.js");
const config = require("../../configs/token.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
const superagent = require("superagent");
const colors = require("../../configs/colors.json");
module.exports = {
    config: {
        name: `docs`,
        category: "",
        description: "",
        aliases: [``],
    },
    run: async(bot, message, args) => {
        try {
            let choice = args[0];

            if (!args[0] || args[0] == undefined) {
                const embed = new MessageEmbed()
                    .setTitle("Documentations and Extra Notations")
                    .setDescription(
                        "You can find extra info here that couldn't make it into the command itself. Check with the command to search for the valid `{tags}`"
                    )
                    .addField("Valid Tags", "```global, bot, service```")
                    .setFooter("Confused? Join the support server!")
                    .setThumbnail(resource.docs);

                message.channel.send({ embed });
            } else if (args[0] == "bot") {
                const embed = new MessageEmbed()
                    .setTitle("Bot Documentations")
                    .setDescription("Sci-Bot is a bot created by user: `exoad`")
                    .addField(
                        "Usage",
                        "This bot's main purpose is of course to give information related to scientific topics."
                    )
                    .addField(
                        "News",
                        "This bot is still in pre-alpha is always being updated with new features"
                    )
                    .addField(
                        "Features",
                        `Prefix: ${config.prefix}\nThis bot is not designed to multitask and perform things outside of its tasks`
                    )
                    .addField("Extra Notes", "All Credits moved to `service` docs")
                    .setThumbnail(resource.docs);

                message.channel.send({ embed });
            } else if (args[0] == "service") {
                const embed = new MessageEmbed()
                    .setTitle("Services and Credits")
                    .addField(
                        "APIs Used",
                        "[NasaAPOD](https://apod.nasa.gov/apod/astropix.html)\n[RandomFact](https://fungenerators.com/api/facts/)\n[DiscordBotLib](https://github.com/yourWaifu/sleepy-discord)\n[LaTeX](https://github.com/latex3/)\n*Note most other APIs are either self made or private projects*"
                    )
                    .setThumbnail(resource.docs);

                message.channel.send({ embed });
            } else {
                const embed = new MessageEmbed()
                    .setTitle("Documentations and Extra Notations")
                    .setDescription(
                        "You can find extra info here that couldn't make it into the command itself. Check with the command to search for the valid `{tags}`"
                    )
                    .addField("Valid Tags", "```global, bot, service```")
                    .setFooter("Confused? Join the support server!")
                    .setThumbnail(resource.docs);

                message.channel.send({ embed });
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