const { MessageEmbed } = require("discord.js");
const config = require("../../../configs/token.json");
const chnl = require("../../../configs/chnl.json");
const resource = require("../../../configs/resource.json");
const superagent = require("superagent");
const colors = require("../../../configs/colors.json");
module.exports = {
    config: {
        name: `math`,
        category: "",
        description: "",
        aliases: [`calc`],
    },
    run: async(bot, message, args) => {
        try {
            let toCalc = args.slice(0).join(" ");
            const body = await superagent.get(
                "https://api.mathjs.org/v4/?expr=" +
                toCalc.replace("+", "%2B").replace("/", "%")
            );
            if (!toCalc || toCalc == undefined) {
                const embed = new MessageEmbed()
                    .setTitle("Oops, looks like you're missing some parameters!")
                    .setDescription("**Usage:** `" + config.prefix + "math {equation}`\n")
                    .addField(
                        "Accepted Parameters for [equation]:",
                        "`operation(values)` => **Operation** factor can be `sqrt`, `add`, `divide`, etc.\n**Values** are numbers to be altered and separated by `,`"
                    )
                    .addField(
                        "Example",
                        "`" +
                        config.prefix +
                        "element Hydrogen`\n**Parameters Taken:** `element_fullname`"
                    )
                    .setFooter("Have questions? Join the support server!")
                    .setThumbnail(resource.aw_snap)
                    .setColor(colors.error);

                message.channel.send({ embed });
            } else {
                const embed = new MessageEmbed()
                    .addField(":inbox_tray: Input", "```css\n" + toCalc + "```")
                    .addField(":outbox_tray: Output", "```css\n" + body.text + "```");

                message.channel.send({ embed });
            }
        } catch (err) {
            console.log(err);
            bot.channels.cache
                .get(chnl.errors)
                .send(`<@709776016314204283>\n \`\`\`console\n${err.stack}\n\`\`\``);
            const embed = new MessageEmbed()
                .setTitle("Aw snap, an internal error!")
                .setThumbnail(resource.aw_snap)
                .setDescription(
                    "Something went wrong with retrieving the info! This issue will be sent to the developer, so please hold on :)"
                )
                .setColor(colors.error);

            message.channel.send({ embed }).then((m) => {
                m.delete({ timeout: 10000 });
            });
        }
    },
};